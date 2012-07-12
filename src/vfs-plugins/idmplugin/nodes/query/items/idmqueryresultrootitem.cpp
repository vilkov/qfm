#include "idmqueryresultrootitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultRootItem::QueryResultRootItem(const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultItem(parent),
	m_value(value)
{}

QVariant QueryResultRootItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool QueryResultRootItem::isRoot()
{
	return true;
}

bool QueryResultRootItem::isProperty()
{
	return false;
}

bool QueryResultRootItem::isValue()
{
	return false;
}

bool QueryResultRootItem::isPath()
{
	return false;
}

bool QueryResultRootItem::isRootPathValue()
{
	return false;
}

IDM_PLUGIN_NS_END
