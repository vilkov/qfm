#include "filesystemrootfolderitem.h"


FILE_SYSTEM_NS_BEGIN

RootFolderItem::RootFolderItem() :
	FolderBaseItem(Info()),
	m_label(QString::fromLatin1(".."))
{}

QString RootFolderItem::fileName() const
{
	return QString();
}

QVariant RootFolderItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool RootFolderItem::isRootItem() const
{
	return true;
}

FILE_SYSTEM_NS_END
