#include "filesystemrootitem.h"


FILE_SYSTEM_NS_BEGIN

FilesystemRootItem::FilesystemRootItem() :
	FileSystemBaseItem(Info()),
	m_label(QString::fromLatin1(".."))
{}

QString FilesystemRootItem::fileName() const
{
	return QString();
}

QVariant FilesystemRootItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool FilesystemRootItem::isRootItem() const
{
	return true;
}

bool FilesystemRootItem::isListItem() const
{
	return false;
}

FILE_SYSTEM_NS_END
