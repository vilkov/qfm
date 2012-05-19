#include "arcnodeentryitem.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeEntryItem::ArcNodeEntryItem(WrappedNodeItem *item, Base *parent) :
	ArcNodeItem(parent),
	m_info(item->info().take()),
	m_totalSize(item->totalSize())
{
	for (WrappedNodeItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		m_items.push_back(NodeItem::Holder(new ArcNodeEntryItem((*i), this)));
}

ArcNodeEntryItem::Base *ArcNodeEntryItem::at(size_type index) const
{
	return m_items.at(index).data();
}

ArcNodeEntryItem::size_type ArcNodeEntryItem::size() const
{
	return m_items.size();
}

ArcNodeEntryItem::size_type ArcNodeEntryItem::indexOf(Base *item) const
{
	ArcNodeItem::Holder holder(static_cast<ArcNodeEntryItem *>(item));
	return m_items.indexOf(holder);
}

QVariant ArcNodeEntryItem::data(qint32 column, qint32 role) const
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

void ArcNodeEntryItem::lock(const QString &reason)
{
	start(m_totalSize);
	TasksNodeItem::lock(reason);
}

ARC_PLUGIN_NS_END
