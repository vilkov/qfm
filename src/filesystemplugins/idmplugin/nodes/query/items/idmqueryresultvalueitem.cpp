#include "idmqueryresultvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultValueItem::QueryResultValueItem(IdmEntityValue *value, IdmItem *parent) :
	IdmItem(parent),
	m_value(value)
{}

QVariant QueryResultValueItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
