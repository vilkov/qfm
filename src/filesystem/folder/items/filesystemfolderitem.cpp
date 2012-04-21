#include "filesystemfolderitem.h"
#include "../../tools/filesystemcommontools.h"


FILE_SYSTEM_NS_BEGIN

FolderItem::FolderItem(const Info &info, Base *parent) :
	FolderBaseItem(info, parent)
{}

FolderItem::FolderItem(const Info &info, Node *node, Base *parent) :
	FolderBaseItem(info, parent)
{
	setNode(node);
}

QVariant FolderItem::data(qint32 column, qint32 role) const
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

bool FolderItem::isRootItem() const
{
	return false;
}

void FolderItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	FolderBaseItem::lock(reason);
	start(totalSize);
}

void FolderItem::lock(const QString &reason)
{
	FolderBaseItem::lock(reason);
}

void FolderItem::unlock()
{
	stop();
	FolderBaseItem::unlock();
}

FILE_SYSTEM_NS_END
