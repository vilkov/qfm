#include "idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultValueItem(value, parent),
	m_info(value->value().toString()),
	m_node(NULL)
{}

IDM_PLUGIN_NS_END
