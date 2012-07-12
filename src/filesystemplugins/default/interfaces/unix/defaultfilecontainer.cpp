#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultcopycontrol.h"
#include "../defaultfileinfo.h"

#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../filesystem/interfaces/filesysteminodeview.h"

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


DEFAULT_PLUGIN_NS_BEGIN

BaseFileContainer::BaseFileContainer(const QByteArray &path) :
	m_path(path)
{}

bool BaseFileContainer::isDefault() const
{
	return true;
}

IFileInfo::size_type BaseFileContainer::freeSpace() const
{
	return Tools::freeSpace(m_path);
}

ICopyControl *BaseFileContainer::createControl(INodeView *view) const
{
	return new CopyControl(view->node(), m_path);
}

QString BaseFileContainer::location() const
{
	return Info::codec()->toUnicode(m_path);
}

bool BaseFileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), &st) == 0;
}

IFileInfo *BaseFileContainer::info(const QString &fileName, QString &error) const
{
	Info info(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), Info::Refresh());

	if (info.exists())
		return new Info(info, Info::Identify());
	else
		error = Info::codec()->toUnicode(::strerror(ENOENT));

	return NULL;
}

bool BaseFileContainer::remove(const IFileInfo *info, QString &error) const
{
#ifdef Q_OS_WIN
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(static_cast<const Info *>(info)->rawFileName());

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

bool BaseFileContainer::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	if (::rename(QByteArray(m_path).append('/').append(static_cast<const Info *>(info)->rawFileName()),
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

bool BaseFileContainer::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	QByteArray destFileName = QByteArray(m_path).append('/').append(static_cast<const Info *>(info)->rawFileName());
	QByteArray sourceFileName = QByteArray(static_cast<const BaseFileContainer *>(source)->m_path).append('/').append(static_cast<const Info *>(info)->rawFileName());

	if (::link(sourceFileName, destFileName) == 0)
		return true;
	else
		if (errno == EEXIST && ::unlink(destFileName) == 0 && ::link(sourceFileName, destFileName) == 0)
			return true;

	error = Info::codec()->toUnicode(::strerror(errno));
	return false;
}

IFileContainer *BaseFileContainer::open() const
{
	return new FileContainer(m_path);
}

IFileContainer *BaseFileContainer::open(const IFileInfo *info, QString &error) const
{
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(static_cast<const Info *>(info)->rawFileName());

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
			return new FileContainer(name);
		else
			errno = ENOTDIR;

	error = Info::codec()->toUnicode(::strerror(errno));
	return NULL;
}

IFileAccessor *BaseFileContainer::open(const IFileInfo *info, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QByteArray(m_path).append('/').append(static_cast<const Info *>(info)->rawFileName()), flags));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = Info::codec()->toUnicode(::strerror(errno));

	return NULL;
}

IFileContainer *BaseFileContainer::create(const QString &fileName, QString &error) const
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

IFileAccessor *BaseFileContainer::create(const QString &fileName, int flags, QString &error) const
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

IFileContainer *BaseFileContainer::filter(Filter::Holder &filter, QString &error) const
{
	return new FilteredFileContainer(m_path, filter);
}


FileContainer::FileContainer(const QByteArray &path) :
	BaseFileContainer(path),
	m_scanner(this)
{}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_scanner;
}


FilteredFileContainer::FilteredFileContainer(const QByteArray &path, Filter::Holder &filter) :
	BaseFileContainer(path),
	m_filter(filter.take()),
	m_scanner(this, m_filter.data())
{}

IFileInfo *FilteredFileContainer::info(const QString &fileName, QString &error) const
{
	Info info(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), Info::Identify());

	if (info.exists() && m_filter->match(&info))
		return new Info(info, Info::None());
	else
		error = Info::codec()->toUnicode(::strerror(ENOENT));

	return NULL;
}

const IFileContainerScanner *FilteredFileContainer::scanner() const
{
	return &m_scanner;
}

DEFAULT_PLUGIN_NS_END
