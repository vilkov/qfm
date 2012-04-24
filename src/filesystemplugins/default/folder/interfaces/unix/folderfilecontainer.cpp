#include "../folderfilecontainer.h"
#include "../folderfileaccessor.h"
#include "../foldercopycontrol.h"
#include "../folderfileinfo.h"
#include "../../model/items/defaultfolderitem.h"

#include "../../../../../filesystem/interfaces/filesysteminodeview.h"
#include "../../../../../filesystem/tools/filesystemcommontools.h"

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
	Enumerator(const QByteArray &path) :
		m_dir(opendir(path)),
		m_path(QByteArray(path).append('/'))
	{}

	virtual ~Enumerator()
	{
		closedir(m_dir);
	}

	virtual bool next()
	{
		if (m_dir)
			while (m_entry = readdir(m_dir))
				if (m_entry->d_type == DT_DIR)
				{
					if (strcmp(m_entry->d_name, ".") != 0 && strcmp(m_entry->d_name, "..") != 0)
					{
						m_info = Info(QByteArray(m_path).append(m_entry->d_name), Info::Identify());
						return true;
					}
				}
				else
				{
					m_info = Info(QByteArray(m_path).append(m_entry->d_name), Info::Identify());
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
	DIR *m_dir;
	Info m_info;
	QByteArray m_path;
	struct dirent *m_entry;
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

IFileContainer *FileContainer::open() const
{
	return new FileContainer(*this);
}

IFileAccessor *FileContainer::open(const QString &fileName, int mode, QString &error) const
{
	FileAccesor::Holder file(new FileAccesor(QString(m_path).append(QChar('/')).append(fileName), mode));

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

void FileContainer::scan(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const
{
	Info info(snapshot.container()->location(file), Info::Refresh());

	if (info.isDir())
	{
		PScopedPointer<InfoListItem> subnode(new InfoListItem(snapshot.container(), file));

		scan(subnode.data(), aborted);

		snapshot.push_back(item, subnode.take());
	}
	else
		snapshot.push_back(item, new InfoItem(snapshot.container(), file));
}

void FileContainer::refresh(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const
{
	Info info(snapshot.container()->location(file), Info::Refresh());

	if (info.isDir())
		snapshot.push_back(item, new InfoListItem(snapshot.container(), file));
	else
		snapshot.push_back(item, new InfoItem(snapshot.container(), file));
}

void FileContainer::scan(InfoListItem *root, const volatile BaseTask::Flags &aborted) const
{
	DIR *dir;

	if (dir = opendir(root->container()->location().toUtf8()))
	{
		struct dirent *entry;

		while ((entry = readdir(dir)) != NULL && !aborted)
			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					PScopedPointer<InfoListItem> subtree(new InfoListItem(root->container(), QString::fromUtf8(entry->d_name)));

					scan(subtree.data(), aborted);
					root->add(subtree.take());
				}
			}
			else
				root->add(new InfoItem(root->container(), QString::fromUtf8(entry->d_name)));

		closedir(dir);
	}
}

DEFAULT_PLUGIN_NS_END
