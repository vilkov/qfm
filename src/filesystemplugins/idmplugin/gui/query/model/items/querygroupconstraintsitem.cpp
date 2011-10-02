#include "querygroupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

QueryGroupConstraintsItem::QueryGroupConstraintsItem(IdmItem *parent) :
	IdmListItem(parent),
	m_value(QString::fromLatin1("(...)"))
{}

QVariant QueryGroupConstraintsItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_value;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
