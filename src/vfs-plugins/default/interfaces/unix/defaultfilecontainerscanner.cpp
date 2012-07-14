#include "../defaultfilecontainerscanner.h"
#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultfileinfo.h"
#include "../../model/items/defaultnodeitem.h"

#include <vfs/containers/vfs_snapshot.h>

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
			if ((m_entry->d_type == DT_DIR || m_entry->d_type == DT_UNKNOWN) &&
				(strcmp(m_entry->d_name, ".") == 0 || strcmp(m_entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				m_info = Info(QByteArray(m_path).append(m_entry->d_name), Info::Identify());

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
			return new FileAccesor(QByteArray(m_path).append(m_info.rawFileName()), mode);
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
			if ((m_entry->d_type == DT_DIR || m_entry->d_type == DT_UNKNOWN) &&
				(strcmp(m_entry->d_name, ".") == 0 || strcmp(m_entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				m_info = Info(QByteArray(m_path).append(m_entry->d_name), Info::Refresh());

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


class WrappedNodeItem : public ::VFS::WrappedNodeItem
{
public:
	WrappedNodeItem(const IFileContainer *container, WrappedNodeItem *parent) :
		::VFS::WrappedNodeItem(container, parent)
	{}
	WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info, WrappedNodeItem *parent) :
		::VFS::WrappedNodeItem(container, info, parent)
	{}
};


FileContainerScanner::FileContainerScanner(const IFileContainer *container) :
	m_container(container)
{}

FileContainerScanner::IEnumerator *FileContainerScanner::enumerate(QString &error) const
{
	return new Enumerator(m_container->location());
}

void FileContainerScanner::scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	IFileInfo::Holder info;
	PScopedPointer<WrappedNodeItem> subnode;

	if (snapshot.isEmpty())
		fill(snapshot, aborted, error);
	else
	{
		QByteArray path = static_cast<const BaseFileContainer *>(snapshot.container())->m_path;
		path.append('/');

		for (Snapshot::iterator it = snapshot.begin(), end = snapshot.end(); it != end && !aborted; ++it)
		{
			info = new Info(QByteArray(path).append((*it).first.as<NodeItem>()->info().as<Info>()->rawFileName()), Info::Identify());

			if (info.as<Info>()->exists())
			{
				subnode = new WrappedNodeItem(snapshot.container(), info, NULL);

				if (subnode->info()->isDir())
				{
					subnode->thisContainer() = subnode->container()->open(subnode->info(), error);

					if (subnode->thisContainer())
						scan(subnode.data(), aborted, error);
				}

				snapshot.insert(it, subnode.take());
			}
		}
	}
}

void FileContainerScanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	IFileInfo::Holder info;
	PScopedPointer<WrappedNodeItem> subnode;

	QByteArray path = static_cast<const BaseFileContainer *>(snapshot.container())->m_path;
	path.append('/');

	for (Snapshot::iterator it = snapshot.begin(), end = snapshot.end(); it != end && !aborted; ++it)
	{
		info = new Info(QByteArray(path).append((*it).first.as<NodeItem>()->info().as<Info>()->rawFileName()), Info::Identify());

		if (info.as<Info>()->exists())
		{
			subnode = new WrappedNodeItem(snapshot.container(), info, NULL);
			snapshot.insert(it, subnode.take());
		}
	}
}

void FileContainerScanner::fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	QByteArray path = static_cast<const BaseFileContainer *>(snapshot.container())->m_path;

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				info = new Info(QByteArray(path).append(entry->d_name), Info::Identify());

				if (info->isDir())
				{
					subtree = new WrappedNodeItem(snapshot.container(), info, NULL);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info(), error))
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
	QByteArray path = root->thisContainer().as<BaseFileContainer>()->m_path;

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				info = new Info(QByteArray(path).append(entry->d_name), Info::Identify());

				if (info->isDir())
				{
					subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info(), error))
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


FilteredFileContainerScanner::FilteredFileContainerScanner(const IFileContainer *container, const IFileContainer::Filter *filter) :
	FileContainerScanner(container),
	m_filter(filter)
{
	Q_ASSERT(m_filter);
}

FilteredFileContainerScanner::IEnumerator *FilteredFileContainerScanner::enumerate(QString &error) const
{
	return new FilteredEnumerator(m_container->location(), m_filter);
}

void FilteredFileContainerScanner::fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	QByteArray path = static_cast<const BaseFileContainer *>(snapshot.container())->m_path;

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		Info tmp;
		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				tmp = Info(QByteArray(path).append(entry->d_name), Info::Identify());

				if (tmp.isDir())
				{
					subtree = new WrappedNodeItem(snapshot.container(), NULL);

					if (subtree->thisContainer() = subtree->container()->open(&tmp, error))
						scan(subtree.data(), aborted, error);

					if (!subtree->isEmpty() || m_filter->match(&tmp))
					{
						subtree->info() = new Info(tmp);
						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
				}
				else
					if (tmp.isFile() && m_filter->match(&tmp))
					{
						info = new Info(tmp);
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
	QByteArray path = root->thisContainer().as<BaseFileContainer>()->m_path;

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		Info tmp;
		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				tmp = Info(QByteArray(path).append(entry->d_name), Info::Identify());

				if (tmp.isDir())
				{
					subtree = new WrappedNodeItem(root->thisContainer().data(), root);

					if (subtree->thisContainer() = subtree->container()->open(&tmp, error))
						scan(subtree.data(), aborted, error);

					if (!subtree->isEmpty() || m_filter->match(&tmp))
					{
						subtree->info() = new Info(tmp);
						root->append(subtree.data());
						subtree.release();
					}
				}
				else
					if (tmp.isFile() && m_filter->match(&tmp))
					{
						info = new Info(tmp);
						subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
						root->append(subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

DEFAULT_PLUGIN_NS_END
