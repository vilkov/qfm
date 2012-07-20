#include "libarchivefileinfo.h"
#include "../../../../application.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

Info::Info()
{}

Info::Info(const Data &data, bool isDir) :
	m_data(data),
	m_isDir(isDir),
	m_fileTypeInfo(Application::desktopService()->fileTypeInfoFromFileName(m_data.fileName, m_isDir))
{}

bool Info::isDir() const
{
	return m_isDir;
}

bool Info::isFile() const
{
	return !m_isDir;
}

bool Info::isLink() const
{
	return false;
}

Info::size_type Info::fileSize() const
{
	return m_data.fileSize;
}

const Location &Info::fileName() const
{
	return m_data.fileName;
}

const IFileType *Info::fileType() const
{
	return this;
}

QDateTime Info::lastModified() const
{
	return m_data.lastModified;
}

int Info::permissions() const
{
	return Read;
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

Location Info::location(const QByteArray &location)
{
	return IFileInfo::location(QString::fromUtf8(location), location);
}

LIBARCHIVE_ARC_PLUGIN_NS_END
