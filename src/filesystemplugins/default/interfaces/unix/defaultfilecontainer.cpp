#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultcopycontrol.h"
#include "../defaultfileinfo.h"

#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../filesystem/containers/filesystemsnapshot.h"
#include "../../../../filesystem/interfaces/filesysteminodeview.h"

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
	union Entry
	{
		struct dirent d;
		char b[offsetof (struct dirent, d_name) + NAME_MAX + 1];
	};

public:
	Enumerator(const QByteArray &path) :
		m_res(NULL),
		m_path(QByteArray(path).append('/')),
		m_dir(opendir(m_path))
	{
		memset(&m_entry, 0, sizeof(Entry));
	}

	virtual ~Enumerator()
	{
		closedir(m_dir);
	}

	virtual bool next()
	{
		while (readdir_r(m_dir, &m_entry.d, &m_res) == 0 && m_res)
			if (m_res->d_type == DT_DIR)
			{
				if (strcmp(m_res->d_name, ".") != 0 && strcmp(m_res->d_name, "..") != 0)
				{
					m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_res->d_name)), Info::Identify());

					if (m_info.isFile() || m_info.isDir())
						return true;
				}
			}
			else
			{
				m_info = Info(QString::fromUtf8(QByteArray(m_path).append(m_res->d_name)), Info::Identify());

				if (m_info.isFile() || m_info.isDir())
					return true;
			}

		return false;
	}

	virtual QString fileName() const
	{
		return m_info.fileName();
	}

	virtual IFileInfo *create() const
	{
		return new Info(m_info);
	}

	virtual bool isObsolete(const IFileInfo *item) const
	{
		return
			item->lastModified() != m_info.lastModified() ||
			item->fileSize() != m_info.fileSize() ||
			item->permissions() != m_info.permissions() ||
			item->fileType()->name().isEmpty();
	}

private:
	struct dirent *m_res;
	Entry m_entry;

	QByteArray m_path;
	Info m_info;
	DIR *m_dir;
};


FileContainer::FileContainer(const QString &path) :
	m_path(path)
{}

QString FileContainer::location() const
{
	return m_path;
}

QString FileContainer::location(const QString &fileName) const
{
	return QString(m_path).append(QChar('/')).append(fileName);
}

bool FileContainer::isPhysical() const
{
	return true;
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	return Tools::freeSpace(m_path.toUtf8());
}

ICopyControl *FileContainer::createControl(INodeView *view) const
{
	return new CopyControl(view->node(), m_path);
}

bool FileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QString(m_path).append(QChar('/')).append(fileName).toUtf8(), &st) == 0;
}

bool FileContainer::remove(const QString &fileName, QString &error) const
{
#ifdef Q_OS_WIN
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif
	struct stat st;
	QByteArray name = QString(m_path).append(QChar('/')).append(fileName).toUtf8();

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
		{
			if (::rmdir(name) == 0)
				return true;
		}
		else
			if (::unlink(name) == 0)
				return true;

	error = QString::fromUtf8(::strerror(errno));
	return false;
}

bool FileContainer::rename(const QString &oldName, const QString &newName, QString &error) const
{
	if (::rename(QString(m_path).append(QChar('/')).append(oldName).toUtf8(), QString(m_path).append(QChar('/')).append(newName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

bool FileContainer::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	QByteArray destFileName = location(fileName).toUtf8();
	QByteArray sourceFileName = source->location(fileName).toUtf8();

	if (::link(sourceFileName, destFileName) == 0)
		return true;
	else
		if (errno == EEXIST && ::unlink(destFileName) == 0 && ::link(sourceFileName, destFileName) == 0)
			return true;

	error = QString::fromUtf8(::strerror(errno));
	return false;
}

IFileContainer *FileContainer::open() const
{
	return new FileContainer(*this);
}

IFileAccessor *FileContainer::open(const QString &fileName, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QString(m_path).append(QChar('/')).append(fileName), flags));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = QString::fromUtf8(::strerror(errno));

	return NULL;
}

IFileContainer *FileContainer::open(const QString &fileName, bool create, QString &error) const
{
	struct stat st;
	QByteArray name = QString(m_path).append(QChar('/')).append(fileName).toUtf8();

	if (::stat(name, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			return new FileContainer(QString::fromUtf8(name));
		else
			errno = ENOTDIR;
	}
	else
		if (errno == ENOENT &&
			create &&
			::mkdir(name, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) == 0)
		{
			return new FileContainer(QString::fromUtf8(name));
		}

	error = QString::fromUtf8(::strerror(errno));
	return NULL;
}

const IFileContainerScanner *FileContainer::scanner() const
{
	return this;
}

void FileContainer::enumerate(IEnumerator::Holder &enumerator) const
{
	enumerator = new Enumerator(m_path.toUtf8());
}

IFileInfo *FileContainer::info(const QString &fileName, QString &error) const
{
	if (contains(fileName))
		return new Info(location(fileName), Info::Identify());
	else
		error = QString::fromUtf8(::strerror(ENOENT));

	return NULL;
}

void FileContainer::scan(Snapshot &snapshot, const volatile Flags &aborted) const
{
	Info raw;
	QString error;
	IFileInfo::Holder info;
	PScopedPointer<WrappedNodeItem> subnode;

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
					scan(subnode.data(), aborted);
			}

			snapshot.insert(it, subnode.take());
		}
	}
}

void FileContainer::refresh(Snapshot &snapshot, const volatile Flags &aborted) const
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

void FileContainer::scan(WrappedNodeItem *root, const volatile Flags &aborted) const
{
	DIR *dir;

	if (dir = opendir(root->thisContainer()->location().toUtf8()))
	{
		QString error;
		struct dirent *entry;
		IFileInfo::Holder info;
		PScopedPointer<WrappedNodeItem> subtree;

		while ((entry = readdir(dir)) != NULL && !aborted)
			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					info = new Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Identify());
					subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);

					if (subtree->thisContainer() = subtree->container()->open(subtree->info()->fileName(), false, error))
						scan(subtree.data(), aborted);

					root->append(subtree.data());
					subtree.take();
				}
			}
			else
			{
				info = new Info(root->thisContainer()->location(QString::fromUtf8(entry->d_name)), Info::Identify());
				subtree = new WrappedNodeItem(root->thisContainer().data(), info, root);
				root->append(subtree.data());
				subtree.take();
			}

		closedir(dir);
	}
}

DEFAULT_PLUGIN_NS_END
