#include "folderinvalidfileinfo.h"


FILE_SYSTEM_NS_BEGIN

InvalidInfo::InvalidInfo()
{}

FileTypeId InvalidInfo::id() const
{
	return FileTypeId();
}

QIcon InvalidInfo::icon() const
{
	return QIcon();
}

QString InvalidInfo::name() const
{
	return QString();
}

QString InvalidInfo::description() const
{
	return QString();
}

bool InvalidInfo::isDir() const
{
	return false;
}

bool InvalidInfo::isFile() const
{
	return false;
}

bool InvalidInfo::isLink() const
{
	return false;
}

InvalidInfo::size_type InvalidInfo::fileSize() const
{
	return 0;
}

QString InvalidInfo::fileName() const
{
	return QString();
}

QDateTime InvalidInfo::lastModified() const
{
	return QDateTime();
}

int InvalidInfo::permissions() const
{
	return 0;
}

FILE_SYSTEM_NS_END
