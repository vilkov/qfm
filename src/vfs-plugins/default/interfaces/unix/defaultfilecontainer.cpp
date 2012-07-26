#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultcopycontrol.h"
#include "../defaultfileinfo.h"

#include <vfs/tools/vfs_commontools.h>
#include <vfs/interfaces/vfs_inodeview.h>

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


DEFAULT_PLUGIN_NS_BEGIN

FileContainer::FileContainer(const QByteArray &path) :
	m_path(IFileContainer::location(Info::codec()->toUnicode(path), path)),
	m_scanner(this)
{}

bool FileContainer::isDefault() const
{
	return true;
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	struct statvfs64 info;

	if (statvfs64(m_path.as<QByteArray>(), &info) == -1)
		return 0;
	else
		return info.f_bsize * info.f_bfree;
}

ICopyControl *FileContainer::createControl(INodeView *view) const
{
	return new CopyControl(view->node(), m_path);
}

const Location &FileContainer::location() const
{
	return m_path;
}

Location FileContainer::location(const IFileInfo *info) const
{
	QByteArray path(m_path);
	path.append('/').append(info->fileName().as<QByteArray>());
	return IFileContainer::location(Info::codec()->toUnicode(path), path);
}

Location FileContainer::location(const QString &fileName) const
{
	QByteArray path(m_path);
	path.append('/');
	return IFileContainer::location(Info::codec()->toUnicode(path).append(fileName), path);
}

bool FileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), &st) == 0;
}

IFileInfo *FileContainer::info(const QString &fileName, QString &error) const
{
	Info info(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), Info::Refresh());

	if (info.exists())
		return new Info(info, Info::Identify());
	else
		error = Info::codec()->toUnicode(::strerror(ENOENT));

	return NULL;
}

bool FileContainer::remove(const IFileInfo *info, QString &error) const
{
#ifdef Q_OS_WIN
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
		{
			if (::rmdir(name) == 0)
				return true;
		}
		else
			if (::unlink(name) == 0)
				return true;

	error = Info::codec()->toUnicode(::strerror(errno));
	return false;
}

bool FileContainer::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	if (::rename(QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>()),
				 QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName))) == 0)
	{
		return true;
	}
	else
	{
		error = Info::codec()->toUnicode(::strerror(errno));
		return false;
	}
}

bool FileContainer::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	QByteArray destFileName = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());
	QByteArray sourceFileName = QByteArray(source->location()).append('/').append(info->fileName().as<QByteArray>());

	if (::link(sourceFileName, destFileName) == 0)
		return true;
	else
		if (errno == EEXIST && ::unlink(destFileName) == 0 && ::link(sourceFileName, destFileName) == 0)
			return true;

	error = Info::codec()->toUnicode(::strerror(errno));
	return false;
}

IFileContainer *FileContainer::open() const
{
	return new FileContainer(m_path);
}

IFileContainer *FileContainer::open(const IFileInfo *info, QString &error) const
{
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
			return new FileContainer(name);
		else
			errno = ENOTDIR;

	error = Info::codec()->toUnicode(::strerror(errno));
	return NULL;
}

IFileAccessor *FileContainer::open(const IFileInfo *info, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>()), flags));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = Info::codec()->toUnicode(::strerror(errno));

	return NULL;
}

IFileContainer *FileContainer::create(const QString &fileName, QString &error) const
{
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName));

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
			return new FileContainer(name);
		else
			errno = ENOTDIR;
	else
		if (errno == ENOENT && ::mkdir(name, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) == 0)
			return new FileContainer(name);

	error = Info::codec()->toUnicode(::strerror(errno));
	return NULL;
}

IFileAccessor *FileContainer::create(const QString &fileName, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), flags | FileAccesor::Create));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = Info::codec()->toUnicode(::strerror(errno));

	return NULL;
}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_scanner;
}

const IApplications *FileContainer::applications() const
{
	return NULL;
}

FileContainer::LinkedList FileContainer::user(const IFileType *fileType) const
{
	return LinkedList();
}

FileContainer::LinkedList FileContainer::system(const IFileType *fileType) const
{
	return LinkedList();
}

DEFAULT_PLUGIN_NS_END
