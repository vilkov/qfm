#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultcopycontrol.h"

#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../filesystem/interfaces/filesysteminodeview.h"

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


DEFAULT_PLUGIN_NS_BEGIN

BaseFileContainer::BaseFileContainer(const QString &path) :
	m_path(path)
{}

QString BaseFileContainer::location() const
{
	return m_path;
}

QString BaseFileContainer::location(const QString &fileName) const
{
	return QString(m_path).append(QChar('/')).append(fileName);
}

bool BaseFileContainer::isPhysical() const
{
	return true;
}

IFileInfo::size_type BaseFileContainer::freeSpace() const
{
	return Tools::freeSpace(m_path.toUtf8());
}

ICopyControl *BaseFileContainer::createControl(INodeView *view) const
{
	return new CopyControl(view->node(), m_path);
}

bool BaseFileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QString(m_path).append(QChar('/')).append(fileName).toUtf8(), &st) == 0;
}

bool BaseFileContainer::remove(const QString &fileName, QString &error) const
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

bool BaseFileContainer::rename(const QString &oldName, const QString &newName, QString &error) const
{
	if (::rename(QString(m_path).append(QChar('/')).append(oldName).toUtf8(), QString(m_path).append(QChar('/')).append(newName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

bool BaseFileContainer::move(const IFileContainer *source, const QString &fileName, QString &error) const
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

IFileContainer *BaseFileContainer::open() const
{
	return new FileContainer(m_path);
}

IFileAccessor *BaseFileContainer::open(const QString &fileName, int flags, QString &error) const
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

IFileContainer *BaseFileContainer::open(const QString &fileName, bool create, QString &error) const
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

IFileContainer *BaseFileContainer::filter(Filter::Holder &filter, QString &error) const
{
	return new FilteredFileContainer(m_path, filter);
}


FileContainer::FileContainer(const QString &path) :
	BaseFileContainer(path),
	m_scanner(this)
{}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_scanner;
}


FilteredFileContainer::FilteredFileContainer(const QString &path, Filter::Holder &filter) :
	BaseFileContainer(path),
	m_scanner(this, filter)
{}

const IFileContainerScanner *FilteredFileContainer::scanner() const
{
	return &m_scanner;
}

DEFAULT_PLUGIN_NS_END
