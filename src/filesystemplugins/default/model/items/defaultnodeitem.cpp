#include "defaultnodeitem.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"


DEFAULT_PLUGIN_NS_BEGIN

DefaultNodeItem::DefaultNodeItem(Base *parent) :
	TasksNodeItem(parent)
{}

DefaultNodeItem::DefaultNodeItem(Node *node, Base *parent) :
	TasksNodeItem(parent),
	m_node(node)
{}

QVariant DefaultNodeItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return fileName();
				case Qt::DecorationRole:
					if (isLocked())
						return lockIcon();
					else
						return fileType()->icon();
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
					if (isFile())
						return Tools::humanReadableSize(fileSize());
					else
						if (m_totalSize.isNull())
							return fileType()->name();
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					return fileType()->name();
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return lastModified();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
			}
			break;
		}
	}

	return QVariant();
}

void DefaultNodeItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	TasksNodeItem::lock(reason);
	start(totalSize);
}

void DefaultNodeItem::lock(const QString &reason)
{
	TasksNodeItem::lock(reason);
}

void DefaultNodeItem::unlock()
{
	stop();
	TasksNodeItem::unlock();
}

DEFAULT_PLUGIN_NS_END
