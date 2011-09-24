#include "groupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

GroupConstraintsItem::GroupConstraintsItem(QueryConstraintsModelItem *parent) :
	QueryConstraintsModelItem(parent)
{}

GroupConstraintsItem::~GroupConstraintsItem()
{
	qDeleteAll(m_items);
}

bool GroupConstraintsItem::isGroup() const
{
	return true;
}

QVariant GroupConstraintsItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_value;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
