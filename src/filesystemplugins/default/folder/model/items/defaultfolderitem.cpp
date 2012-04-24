#include "defaultfolderitem.h"


DEFAULT_PLUGIN_NS_BEGIN

DefaultFolderItem::DefaultFolderItem(const Info &info, Base *parent) :
	DefaultNodeItem(parent)
{}

DefaultFolderItem::DefaultFolderItem(const Info &info, Node *node, Base *parent) :
	DefaultNodeItem(node, parent)
{}

bool DefaultFolderItem::isDir() const
{
	return m_info.isDir();
}

bool DefaultFolderItem::isFile() const
{
	return m_info.isFile();
}

bool DefaultFolderItem::isLink() const
{
	return m_info.isLink();
}

IFileInfo::size_type DefaultFolderItem::fileSize() const
{
	return m_info.fileSize();
}

QString DefaultFolderItem::fileName() const
{
	return m_info.fileName();
}

const IFileType *DefaultFolderItem::fileType() const
{
	return m_info.fileType();
}

QDateTime DefaultFolderItem::lastModified() const
{
	return m_info.lastModified();
}

int DefaultFolderItem::permissions() const
{
	return m_info.permissions();
}

DEFAULT_PLUGIN_NS_END
