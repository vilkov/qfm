#include "../defaultfilecontainerscanner.h"
#include "../defaultfileaccessor.h"
#include "../defaultfileinfo.h"

#include "../../../../filesystem/containers/filesystemsnapshot.h"

#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


DEFAULT_PLUGIN_NS_BEGIN

class Enumerator : public IFileContainerScanner::IEnumerator
{
public:
	union Buffer
	{
		struct dirent d;
		char b[offsetof(struct dirent, d_name) + NAME_MAX + 1];
	};

public:
	Enumerator(const QByteArray &path) :
		m_entry(NULL),
		m_path(QByteArray(path).append('/')),
		m_dir(opendir(m_path))
	{
		memset(&m_buffer, 0, sizeof(Buffer));
	}

	virtual ~Enumerator()
	{
		closedir(m_dir);
	}

	virtual const IFileInfo *next()
	{
		while (readdir_r(m_dir, &m_buffer.d, &m_entry) == 0 && m_entry)
			if (m_entry->d_type == DT_DIR || m_entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(m_entry->d_name, ".") != 0 && strcmp(m_entry->d_name, "..") != 0)
				{
					m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_entry->d_name)), Info::Identify());

					if (m_info.isFile() || m_info.isDir())
						return &m_info;
				}
			}
			else
			{
				m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_entry->d_name)), Info::Identify());

				if (m_info.isFile() || m_info.isDir())
					return &m_info;
			}

		return NULL;
	}

	virtual IFileInfo *info() const
	{
		return new Info(m_info);
	}

	virtual IFileAccessor *open(int mode, QString &error) const
	{
		if (m_info.isDir())
		{
			errno = ENOENT;
			error = QString::fromUtf8(::strerror(errno));

			return NULL;
		}
		else
			return new FileAccesor(QString::fromUtf8(m_path).append(m_info.fileName()), mode);
	}

protected:
	struct dirent *m_entry;
	Buffer m_buffer;

	QByteArray m_path;
	Info m_info;
	DIR *m_dir;
};


class FilteredEnumerator : public Enumerator
{
public:
	FilteredEnumerator(const QByteArray &path, const IFileContainer::Filter *filter) :
		Enumerator(path),
		m_filter(filter)
	{}

	virtual const IFileInfo *next()
	{
		while (readdir_r(m_dir, &m_buffer.d, &m_entry) == 0 && m_entry)
			if (m_entry->d_type == DT_DIR || m_entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(m_entry->d_name, ".") != 0 && strcmp(m_entry->d_name, "..") != 0)
				{
					m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_entry->d_name)), Info::Refresh());

					if ((m_info.isFile() || m_info.isDir()) && m_filter->match(&m_info))
					{
						m_info = Info(m_info, Info::Identify());
						return &m_info;
					}
				}
			}
			else
			{
				m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_entry->d_name)), Info::Refresh());

				if ((m_info.isFile() || m_info.isDir()) && m_filter->match(&m_info))
				{
					m_info = Info(m_info, Info::Identify());
					return &m_info;
				}
			}

		return NULL;
	}

private:
	const IFileContainer::Filter *m_filter;
};


class WrappedNodeItem : public FileSystem::WrappedNodeItem
{
public:
	WrappedNodeItem(const IFileContainer *container, WrappedNodeItem *parent) :
		FileSystem::WrappedNodeItem(container, parent)
	{}
	WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info, WrappedNodeItem *parent) :
		FileSystem::WrappedNodeItem(container, info, parent)
	{}
};


FileContainerScanner::FileContainerScanner(const IFileContainer *container) :
	m_container(container)
{}

FileContainerScanner::IEnumerator *FileContainerScanner::enumerate(QString &error) const
{
	return new Enumerator(m_container->location().toUtf8());
}

IFileInfo *FileContainerScanner::info(const QString &fileName, QString &error) const
{
	if (m_container->contains(fileName))
		return new Info(m_container->location(fileName), Info::Identify());
	else
		error = QString::fromUtf8(::strerror(ENOENT));

	return NULL;
}

void FileContainerScanner::scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	Info raw;
	IFileInfo::Holder info;
	PScopedPointer<WrappedNodeItem> subnode;

	if (snapshot.isEmpty())
		fill(snapshot, aborted, error);
	else
		for (Snapshot::iterator it = snapshot.begin(), end = snapshot.end(); it != end && !aborted; ++it)
		{
			raw = Info(snapshot.container()->location(it.key()), Info::Identify());

			if (raw.exists())
			{
				info = new Info(raw);
				subnode = new WrappedNodeItem(snapshot.container(), info, NULL);

				if (subnode->info()->isDir())
				{
					subnode->thisContainer() = subnode->container()->open(it.key(), false, error);

					if (subnode->thisContainer())
						scan(subnode.data(), aborted, error);
				}

				snapshot.insert(it, subnode.take());
			}
		}
}

void FileContainerScanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	Info raw;
	IFileInfo::Holder info;
	PScopedPointer<WrappedNodeItem> subnode;

	for (Snapshot::iterator it = snapshot.begin(), end = snapshot.end(); it != end && !aborted; ++it)
	{
		raw = Info(snapshot.container()->location(it.key()), Info::Identify());

		if (raw.exists())
		{
			info = new Info(raw);
			subnode = new WrappedNodeItem(snapshot.container(), info, NULL);
			snapshot.insert(it, subnode.take());
		}
	}
}

void FileContainerScanner::fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	if (DIR *dir = opendir(snapshot.container()->location().toUtf8()))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if (entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					info = new Info(snapshot.container()->location(QString::fromUtf8(entry->d_name)), Info::Identify());

					if (info->isDir())
					{
						subtree = new WrappedNodeItem(snapshot.container(), info, NULL);

						if (subtree->thisContainer() = subtree->container()->open(subtree->info()->fileName(), false, error))
							scan(subtree.data(), aborted, error);

						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
					else
						if (info->isFile())
						{
							subtree = new WrappedNodeItem(snapshot.container(), info, NULL);
							snapshot.insert(subtree->info()->fileName(), subtree.data());
							subtree.release();
						}
				}
			}
			else
			{
				info = new Info(snapshot.container()->location(QString::fromUtf8(entry->d_name)), Info::Identify());

				if (info->isDir())
				{
					subtree = new WrappedNodeItem(snapshot.container(), info, NULL);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info()->fileName(), false, error))
						scan(subtree.data(), aborted, error);

					snapshot.insert(subtree->info()->fileName(), subtree.data());
					subtree.release();
				}
				else
					if (info->isFile())
					{
						subtree = new WrappedNodeItem(snapshot.container(), info, NULL);
						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

void FileContainerScanner::scan(WrappedNodeItem *root, const volatile Flags &aborted, QString &error) const
{
	if (DIR *dir = opendir(root->thisContainer()->location().toUtf8()))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if (entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					info = new Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Identify());

					if (info->isDir())
					{
						subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);

						if (subtree->thisContainer() = subtree->container()->open(subtree->info()->fileName(), false, error))
							scan(subtree.data(), aborted, error);

						root->append(subtree.data());
						subtree.release();
					}
					else
						if (info->isFile())
						{
							subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
							root->append(subtree.data());
							subtree.release();
						}
				}
			}
			else
			{
				info = new Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Identify());

				if (info->isDir())
				{
					subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info()->fileName(), false, error))
						scan(subtree.data(), aborted, error);

					root->append(subtree.data());
					subtree.release();
				}
				else
					if (info->isFile())
					{
						subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
						root->append(subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}


FilteredFileContainerScanner::FilteredFileContainerScanner(const IFileContainer *container, IFileContainer::Filter::Holder &filter) :
	FileContainerScanner(container),
	m_filter(filter.take())
{
	Q_ASSERT(m_filter.data());
}

FilteredFileContainerScanner::IEnumerator *FilteredFileContainerScanner::enumerate(QString &error) const
{
	return new FilteredEnumerator(m_container->location().toUtf8(), m_filter.data());
}

IFileInfo *FilteredFileContainerScanner::info(const QString &fileName, QString &error) const
{
	Info info(m_container->location(fileName), Info::Refresh());

	if (info.exists() && m_filter->match(&info))
		return new Info(info, Info::Identify());
	else
		error = QString::fromUtf8(::strerror(ENOENT));

	return NULL;
}

void FilteredFileContainerScanner::fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	if (DIR *dir = opendir(snapshot.container()->location().toUtf8()))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		Info tmp;
		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if (entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					tmp = Info(snapshot.container()->location(QString::fromUtf8(entry->d_name)), Info::Refresh());

					if (tmp.isDir())
					{
						subtree = new WrappedNodeItem(snapshot.container(), NULL);

						if (subtree->thisContainer() = subtree->container()->open(tmp.fileName(), false, error))
							scan(subtree.data(), aborted, error);

						if (!subtree->isEmpty() || m_filter->match(&tmp))
						{
							subtree->info() = new Info(tmp, Info::Identify());
							snapshot.insert(subtree->info()->fileName(), subtree.data());
							subtree.release();
						}
					}
					else
						if (tmp.isFile() && m_filter->match(&tmp))
						{
							info = new Info(tmp, Info::Identify());
							subtree = new WrappedNodeItem(snapshot.container(), info, NULL);
							snapshot.insert(subtree->info()->fileName(), subtree.data());
							subtree.release();
						}
				}
			}
			else
			{
				tmp = Info(snapshot.container()->location(QString::fromUtf8(entry->d_name)), Info::Refresh());

				if (tmp.isDir())
				{
					subtree = new WrappedNodeItem(snapshot.container(), NULL);

					if (subtree->thisContainer() = subtree->container()->open(tmp.fileName(), false, error))
						scan(subtree.data(), aborted, error);

					if (!subtree->isEmpty() || m_filter->match(&tmp))
					{
						subtree->info() = new Info(tmp, Info::Identify());
						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
				}
				else
					if (tmp.isFile() && m_filter->match(&tmp))
					{
						info = new Info(tmp, Info::Identify());
						subtree = new WrappedNodeItem(snapshot.container(), info, NULL);
						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

void FilteredFileContainerScanner::scan(WrappedNodeItem *root, const volatile Flags &aborted, QString &error) const
{
	if (DIR *dir = opendir(root->thisContainer()->location().toUtf8()))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		Info tmp;
		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if (entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					tmp = Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Refresh());

					if (tmp.isDir())
					{
						subtree = new WrappedNodeItem(root->thisContainer().data(), root);

						if (subtree->thisContainer() = subtree->container()->open(tmp.fileName(), false, error))
							scan(subtree.data(), aborted, error);

						if (!subtree->isEmpty() || m_filter->match(&tmp))
						{
							subtree->info() = new Info(tmp, Info::Identify());
							root->append(subtree.data());
							subtree.release();
						}
					}
					else
						if (tmp.isFile() && m_filter->match(&tmp))
						{
							info = new Info(tmp, Info::Identify());
							subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
							root->append(subtree.data());
							subtree.release();
						}
				}
			}
			else
			{
				tmp = Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Refresh());

				if (tmp.isDir())
				{
					subtree = new WrappedNodeItem(root->thisContainer().data(), root);

					if (subtree->thisContainer() = subtree->container()->open(tmp.fileName(), false, error))
						scan(subtree.data(), aborted, error);

					if (!subtree->isEmpty() || m_filter->match(&tmp))
					{
						subtree->info() = new Info(tmp, Info::Identify());
						root->append(subtree.data());
						subtree.release();
					}
				}
				else
					if (tmp.isFile() && m_filter->match(&tmp))
					{
						info = new Info(tmp, Info::Identify());
						subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
						root->append(subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

DEFAULT_PLUGIN_NS_END
