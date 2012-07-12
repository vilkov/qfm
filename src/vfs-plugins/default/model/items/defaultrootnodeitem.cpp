#include "defaultrootnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN
static IFileInfo::Holder emptyInfo;


RootNodeItem::RootNodeItem(Base *parent) :
	NodeItem(emptyInfo, parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant RootNodeItem::data(qint32 column, qint32 role) const
{
	if (column == 0)
		if (role == Qt::DisplayRole)
			return m_label;
		else
			if (isLocked())
				if (role == Qt::DecorationRole)
					return lockIcon();
				else
					if (role == Qt::ToolTipRole)
						return lockReason();

	return QVariant();
}

bool RootNodeItem::isRootItem() const
{
	return true;
}

DEFAULT_PLUGIN_NS_END
