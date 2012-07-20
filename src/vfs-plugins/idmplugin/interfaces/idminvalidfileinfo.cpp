#include "idminvalidfileinfo.h"
#include "../../../application.h"


IDM_PLUGIN_NS_BEGIN

InvalidInfo::InvalidInfo(const QString &fileName) :
	m_fileName(IFileInfo::location(fileName, fileName.toUtf8())),
	m_icon(Application::desktopService()->missingIcon())
{}

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

const Location &InvalidInfo::fileName() const
{
	return m_fileName;
}

const IFileType *InvalidInfo::fileType() const
{
	return this;
}

QDateTime InvalidInfo::lastModified() const
{
	return QDateTime();
}

int InvalidInfo::permissions() const
{
	return 0;
}

FileTypeId InvalidInfo::id() const
{
	return FileTypeId();
}

QIcon InvalidInfo::icon() const
{
	return m_icon;
}

QString InvalidInfo::name() const
{
	return QString();
}

QString InvalidInfo::description() const
{
	return QString();
}

IDM_PLUGIN_NS_END
