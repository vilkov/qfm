#include "filesystementryitem.h"
#include "../../tools/filesystemcommontools.h"


FILE_SYSTEM_NS_BEGIN

FileSystemEntryItem::FileSystemEntryItem(const Info &info, Base *parent) :
	FileSystemBaseItem(info, parent)
{}

FileSystemEntryItem::FileSystemEntryItem(const Info &info, Node *node, Base *parent) :
	FileSystemBaseItem(info, parent)
{
	setNode(node);
}

QVariant FileSystemEntryItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return info().fileName();
				case Qt::DecorationRole:
					if (isLocked())
						return lockIcon();
					else
						return info().icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					if (isLocked())
						return lockReason();
					else
						break;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					if (info().isFile())
						return Tools::humanReadableSize(info().fileSize());
					else
						if (m_totalSize.isNull())
							return info().name();
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					return info().name();
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return info().lastModified();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
			}
			break;
		}
	}

	return QVariant();
}

bool FileSystemEntryItem::isRootItem() const
{
	return false;
}

void FileSystemEntryItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	FileSystemBaseItem::lock(reason);
	start(totalSize);
}

void FileSystemEntryItem::lock(const QString &reason)
{
	FileSystemBaseItem::lock(reason);
}

void FileSystemEntryItem::unlock()
{
	stop();
	FileSystemBaseItem::unlock();
}

FILE_SYSTEM_NS_END
