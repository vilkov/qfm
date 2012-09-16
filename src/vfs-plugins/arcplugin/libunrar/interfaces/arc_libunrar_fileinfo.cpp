#include "arc_libunrar_fileinfo.h"
#include "../../../default/interfaces/default_fileinfo.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

Info::Info(const Data &data, bool isDir) :
	m_data(data),
	m_isDir(isDir),
	m_fileTypeInfo(Default::Info::fileTypeInfoFromFileName(m_data.fileName, m_isDir))
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

const QDateTime &Info::lastModified() const
{
	return m_data.lastModified;
}

int Info::permissions() const
{
	return 0;
}

const FileTypeId &Info::id() const
{
	return m_fileTypeInfo.id;
}

const QIcon &Info::icon() const
{
	return m_fileTypeInfo.icon;
}

const QString &Info::name() const
{
	return m_fileTypeInfo.name;
}

const QString &Info::description() const
{
	return m_fileTypeInfo.description;
}

Location Info::location(const QString &label)
{
	return IFileInfo::location(label, label.toUtf8());
}

ARC_PLUGIN_LIBUNRAR_NS_END
