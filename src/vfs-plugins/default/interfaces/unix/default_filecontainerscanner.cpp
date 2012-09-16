#include "../default_filecontainerscanner.h"
#include "../default_filecontainer.h"
#include "../default_fileaccessor.h"
#include "../default_fileinfo.h"
#include "../../model/items/default_nodeitem.h"

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
	Enumerator(const QByteArray &path, const IFileContainer *container) :
		m_container(container),
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
				m_info = Info(QByteArray(m_path).append(m_entry->d_name), m_container);

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
			return new FileAccesor(QByteArray(m_path).append(m_info.fileName().as<QByteArray>()), mode);
	}

protected:
	const IFileContainer *m_container;
	struct dirent *m_entry;
	Buffer m_buffer;

	QByteArray m_path;
	Info m_info;
	DIR *m_dir;
};


//class FilteredEnumerator : public Enumerator
//{
//public:
//	FilteredEnumerator(const QByteArray &path, const IFileContainer::Filter *filter) :
//		Enumerator(path),
//		m_filter(filter)
//	{}
//
//	virtual const IFileInfo *next()
//	{
//		while (readdir_r(m_dir, &m_buffer.d, &m_entry) == 0 && m_entry)
//			if ((m_entry->d_type == DT_DIR || m_entry->d_type == DT_UNKNOWN) &&
//				(strcmp(m_entry->d_name, ".") == 0 || strcmp(m_entry->d_name, "..") == 0))
//			{
//				continue;
//			}
//			else
//			{
//				m_info = Info(QByteArray(m_path).append(m_entry->d_name), Info::Identify());
//
//				if ((m_info.isFile() || m_info.isDir()) && m_filter->match(&m_info))
//					return &m_info;
//			}
//
//		return NULL;
//	}
//
//private:
//	const IFileContainer::Filter *m_filter;
//};


class SnapshotItem : public ::VFS::SnapshotItem
{
public:
	SnapshotItem(const IFileContainer *container, SnapshotItem *parent) :
		::VFS::SnapshotItem(container, parent)
	{}
	SnapshotItem(const IFileContainer *container, IFileInfo::Holder &info, SnapshotItem *parent) :
		::VFS::SnapshotItem(container, info, parent)
	{}
};


class SearchSnapshotItem : public ::VFS::SnapshotItem
{
public:
	SearchSnapshotItem(const IFileContainer *container, IFileInfo::Holder &info) :
		::VFS::SnapshotItem(container, info, NULL)
	{}
	SearchSnapshotItem(IFileContainer::Holder &container, IFileInfo::Holder &info) :
		::VFS::SnapshotItem(container.data(), info, NULL),
		m_thisContainer(container.take())
	{}

private:
	IFileContainer::Holder m_thisContainer;
};


FileContainerScanner::FileContainerScanner(const IFileContainer *container) :
	m_container(container)
{}

FileContainerScanner::IEnumerator *FileContainerScanner::enumerate(QString &error) const
{
	return new Enumerator(m_container->location(), m_container);
}

void FileContainerScanner::scan(const ScanArguments &arguments, QString &error) const
{
	IFileInfo::Holder info;
	PScopedPointer<SnapshotItem> subnode;

	if (arguments.snapshot.isEmpty())
		fill(arguments.snapshot, arguments.aborted, error);
	else
	{
		QByteArray path = arguments.snapshot.container()->location();
		path.append('/');

		for (Snapshot::iterator it = arguments.snapshot.begin(), end = arguments.snapshot.end(); it != end && !arguments.aborted; ++it)
		{
			info = new Info(QByteArray(path).append(it.key().as<QByteArray>()), m_container);

			if (info.as<Info>()->exists())
			{
				subnode = new SnapshotItem(arguments.snapshot.container(), info, NULL);

				if (subnode->info()->isDir())
				{
					subnode->thisContainer() = subnode->container()->open(subnode->info(), error);

					if (subnode->thisContainer())
						scan(subnode.data(), arguments.aborted, error);
				}

				arguments.snapshot.insert(it, subnode.take());
			}
		}
	}
}

void FileContainerScanner::search(const SearchArguments &arguments, QString &error) const
{
	if (arguments.snapshot.isEmpty())
		search(arguments.snapshot.container(), arguments.callback, arguments.filter, arguments.aborted);
	else
	{
		Info info;
		QByteArray dir;
		QList<QByteArray> dirs;
		IFileInfo::Holder infoHolder;
		IFileContainer::Holder container;

		QByteArray path = arguments.snapshot.container()->location();
		path.append('/');

		for (Snapshot::iterator it = arguments.snapshot.begin(), end = arguments.snapshot.end(); it != end && !arguments.aborted; ++it)
		{
			info = Info(dir = QByteArray(path).append(it.key().as<QByteArray>()), m_container);

			if (info.isDir())
			{
				dirs.push_back(dir);

				if (arguments.filter->match(&info))
				{
					infoHolder = new Info(info);
					arguments.callback.method(arguments.callback.user_data, new SearchSnapshotItem(arguments.snapshot.container(), infoHolder));
				}
			}
			else
				if (info.isFile() && arguments.filter->match(&info))
				{
					infoHolder = new Info(info);
					arguments.callback.method(arguments.callback.user_data, new SearchSnapshotItem(arguments.snapshot.container(), infoHolder));
				}
		}

		for (QList<QByteArray>::size_type i = 0, size = dirs.size(); i < size && !arguments.aborted; ++i)
		{
			FileContainer container(dirs.at(i));
			search(&container, arguments.callback, arguments.filter, arguments.aborted);
		}
	}
}

void FileContainerScanner::fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	QByteArray path = snapshot.container()->location();

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<SnapshotItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				info = new Info(QByteArray(path).append(entry->d_name), m_container);

				if (info->isDir())
				{
					subtree = new SnapshotItem(snapshot.container(), info, NULL);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info(), error))
						scan(subtree.data(), aborted, error);

					snapshot.insert(subtree->info()->fileName(), subtree.data());
					subtree.release();
				}
				else
					if (info->isFile())
					{
						subtree = new SnapshotItem(snapshot.container(), info, NULL);
						snapshot.insert(subtree->info()->fileName(), subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

void FileContainerScanner::scan(SnapshotItem *root, const volatile Flags &aborted, QString &error) const
{
	QByteArray path = root->thisContainer()->location();

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		IFileInfo::Holder info;
		PScopedPointer<SnapshotItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				info = new Info(QByteArray(path).append(entry->d_name), m_container);

				if (info->isDir())
				{
					subtree = new SnapshotItem(root->thisContainer().data(), info, root);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info(), error))
						scan(subtree.data(), aborted, error);

					root->append(subtree.data());
					subtree.release();
				}
				else
					if (info->isFile())
					{
						subtree = new SnapshotItem(root->thisContainer().data(), info, root);
						root->append(subtree.data());
						subtree.release();
					}
			}

		closedir(dir);
	}
}

void FileContainerScanner::search(const IFileContainer *container, const Callback &callback, const IFileContainerScanner::Filter *filter, const volatile Flags &aborted) const
{
	QByteArray path = container->location();

	if (DIR *dir = opendir(path))
	{
		struct dirent *entry;
		Enumerator::Buffer buffer;

		Info info;
		QByteArray dirPath;
		QList<QByteArray> dirs;
		IFileInfo::Holder infoHolder;
		IFileContainer::Holder containerHolder;
		PScopedPointer<SnapshotItem> subtree;

		path.append('/');

		while (!aborted && readdir_r(dir, &buffer.d, &entry) == 0 && entry)
			if ((entry->d_type == DT_DIR || entry->d_type == DT_UNKNOWN) &&
				(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
			{
				continue;
			}
			else
			{
				info = Info(dirPath = QByteArray(path).append(entry->d_name), m_container);

				if (info.isDir())
				{
					dirs.push_back(dirPath);

					if (filter->match(&info))
					{
						infoHolder = new Info(info);
						containerHolder = container->open();
						callback.method(callback.user_data, new SearchSnapshotItem(containerHolder, infoHolder));
					}
				}
				else
					if (info.isFile() && filter->match(&info))
					{
						infoHolder = new Info(info);
						containerHolder = container->open();
						callback.method(callback.user_data, new SearchSnapshotItem(containerHolder, infoHolder));
					}
			}

		closedir(dir);

		for (QList<QByteArray>::size_type i = 0, size = dirs.size(); i < size && !aborted; ++i)
		{
			FileContainer container(dirs.at(i));
			search(&container, callback, filter, aborted);
		}
	}
}

DEFAULT_PLUGIN_NS_END
