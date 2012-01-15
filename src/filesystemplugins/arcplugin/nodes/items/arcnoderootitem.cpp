#include "arcnoderootitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeRootItem::ArcNodeRootItem(Base *parent) :
	ArcNodeItem(parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant ArcNodeRootItem::data(qint32 column, qint32 role) const
{
	if (column == 0)
		switch (role)
		{
			case Qt::EditRole:
			case Qt::DisplayRole:
				return m_label;

			case Qt::DecorationRole:
				if (isLocked())
					return lockIcon();
				else
					break;

			case Qt::TextAlignmentRole:
				return Qt::AlignLeft;

			case Qt::ToolTipRole:
				if (isLocked())
					return lockReason();
				else
					break;
		}

	return QVariant();
}

bool ArcNodeRootItem::isRoot() const
{
	return true;
}

bool ArcNodeRootItem::isDir() const
{
	return false;
}

ARC_PLUGIN_NS_END
