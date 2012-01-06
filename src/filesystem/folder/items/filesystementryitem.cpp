#include "filesystementryitem.h"
#include "../../tools/filesystemcommontools.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FileSystemEntryItem::FileSystemEntryItem(const Info &info, Base *parent) :
	FileSystemBaseItem(info, parent),
	m_locked(false)
{}

FileSystemEntryItem::FileSystemEntryItem(const Info &info, Node *node, Base *parent) :
	FileSystemBaseItem(info, parent),
	m_locked(false)
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
					if (m_locked)
						return Application::style()->standardIcon(QStyle::SP_BrowserReload);
					else
						return info().icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					if (m_locked)
						return m_lockReason;
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
							return QString::fromLatin1("<DIR>");
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
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
//					case Qt::DecorationRole:
//						return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
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

bool FileSystemEntryItem::isListItem() const
{
	return false;
}

void FileSystemEntryItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	FileSystemBaseItem::lock(reason);
}

void FileSystemEntryItem::lock(const QString &reason)
{
	FileSystemBaseItem::lock(reason);
}

void FileSystemEntryItem::unlock()
{
	FileSystemBaseItem::unlock();
	m_doneSize.clear();
	m_timeElapsed.clear();
}

FILE_SYSTEM_NS_END
