#include "arcentrynodeitem.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"


ARC_PLUGIN_NS_BEGIN

EntryNodeItem::EntryNodeItem(WrappedNodeItem *item, Base *parent) :
	NodeItem(parent),
	m_info(item->info().take()),
	m_totalSize(item->totalSize())
{
	for (WrappedNodeItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		m_items.push_back(NodeItem::Holder(new EntryNodeItem((*i), this)));
}

EntryNodeItem::Base *EntryNodeItem::at(size_type index) const
{
	return m_items.at(index).data();
}

EntryNodeItem::size_type EntryNodeItem::size() const
{
	return m_items.size();
}

EntryNodeItem::size_type EntryNodeItem::indexOf(Base *item) const
{
	Holder holder(static_cast<EntryNodeItem *>(item));
	return m_items.indexOf(holder);
}

QVariant EntryNodeItem::data(qint32 column, qint32 role) const
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
					return Tools::humanReadableSize(m_info->fileSize());

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

void EntryNodeItem::lock(const QString &reason)
{
	start(m_totalSize);
	TasksNodeItem::lock(reason);
}

ARC_PLUGIN_NS_END
