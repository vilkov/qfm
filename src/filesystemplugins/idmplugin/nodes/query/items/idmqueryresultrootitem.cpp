#include "idmqueryresultrootitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultRootItem::QueryResultRootItem(IdmEntityValue *value, IdmItem *parent) :
	QueryResultItem(value, parent),
	m_value(value)
{}

IDM_PLUGIN_NS_END
