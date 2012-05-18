#include "arcfileinfo.h"

#include <archive.h>
#include <archive_entry.h>


ARC_PLUGIN_NS_BEGIN

Info::Info(const QString &fileName, struct archive_entry *entry)
{
	archive_entry_size(entry);
	archive_entry_pathname(entry);
	QDateTime::fromTime_t(archive_entry_mtime(entry));
}

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
	return 0;
}

QString Info::fileName() const
{
	return QString();
}

const IFileType *Info::fileType() const
{
	return this;
}

QDateTime Info::lastModified() const
{
	return QDateTime();
}

int Info::permissions() const
{
	return 0;
}

FileTypeId Info::id() const
{
	return FileTypeId();
}

QIcon Info::icon() const
{
	return QIcon();
}

QString Info::name() const
{
	return QString();
}

QString Info::description() const
{
	return QString();
}

ARC_PLUGIN_NS_END
