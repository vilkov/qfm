#include "idminvalidfileinfo.h"
#include <desktop/theme/desktop_theme.h>


IDM_PLUGIN_NS_BEGIN

InvalidInfo::InvalidInfo(const QString &fileName) :
	m_fileName(IFileInfo::location(fileName, fileName.toUtf8())),
	m_icon(::Desktop::Theme::current()->missingIcon())
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

const QDateTime &InvalidInfo::lastModified() const
{
	return m_lastModified;
}

int InvalidInfo::permissions() const
{
	return 0;
}

const FileTypeId &InvalidInfo::id() const
{
	return m_id;
}

const QIcon &InvalidInfo::icon() const
{
	return m_icon;
}

const QString &InvalidInfo::name() const
{
	return m_name;
}

const QString &InvalidInfo::description() const
{
	return m_description;
}

IDM_PLUGIN_NS_END
