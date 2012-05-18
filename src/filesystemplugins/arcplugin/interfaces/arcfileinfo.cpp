#include "arcfileinfo.h"
#include "../../../application.h"

#include <archive.h>
#include <archive_entry.h>


ARC_PLUGIN_NS_BEGIN

Info::Info(const QString &fileName, struct archive_entry *entry) :
	m_path(archive_entry_pathname(entry)),
	m_fileName(fileName),
	m_fileSize(archive_entry_size(entry)),
	m_lastModified(QDateTime::fromTime_t(archive_entry_mtime(entry))),
	m_fileTypeInfo(Application::desktopService()->fileTypeInfoFromFileName(fileName, m_path.endsWith('/'), 16))
{}

bool Info::isDir() const
{
	return false;
}

bool Info::isFile() const
{
	return false;
}

bool Info::isLink() const
{
	return false;
}

Info::size_type Info::fileSize() const
{
	return m_fileSize;
}

QString Info::fileName() const
{
	return m_fileName;
}

const IFileType *Info::fileType() const
{
	return this;
}

QDateTime Info::lastModified() const
{
	return m_lastModified;
}

int Info::permissions() const
{
	return 0;
}

FileTypeId Info::id() const
{
	return m_fileTypeInfo.id;
}

QIcon Info::icon() const
{
	return m_fileTypeInfo.icon;
}

QString Info::name() const
{
	return m_fileTypeInfo.name;
}

QString Info::description() const
{
	return m_fileTypeInfo.description;
}

ARC_PLUGIN_NS_END
