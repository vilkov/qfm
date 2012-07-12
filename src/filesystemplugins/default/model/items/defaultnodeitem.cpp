#include "defaultnodeitem.h"
#include <vfs/tools/vfs_commontools.h>


DEFAULT_PLUGIN_NS_BEGIN

NodeItem::NodeItem(IFileInfo::Holder &info, Base *parent) :
	TasksNodeItem(parent),
	m_node(NULL),
	m_info(info.take())
{}

NodeItem::NodeItem(IFileInfo::Holder &info, Node *node, Base *parent) :
	TasksNodeItem(parent),
	m_node(node),
	m_info(info.take())
{}

QVariant NodeItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info->fileName();
				case Qt::DecorationRole:
					if (isLocked())
						return lockIcon();
					else
						return m_info->fileType()->icon();
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
					if (m_info->isFile())
						return Tools::humanReadableSize(m_info->fileSize());
					else
						if (m_totalSize.isNull())
							return m_info->fileType()->name();
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					return m_info->fileType()->name();
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info->lastModified();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
			}
			break;
		}
	}

	return QVariant();
}

bool NodeItem::isRootItem() const
{
	return false;
}

void NodeItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	TasksNodeItem::lock(reason);
	start(totalSize);
}

void NodeItem::lock(const QString &reason)
{
	TasksNodeItem::lock(reason);
}

void NodeItem::unlock()
{
	stop();
	TasksNodeItem::unlock();
}

DEFAULT_PLUGIN_NS_END
