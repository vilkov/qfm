#include "idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

IQueryResultItem::~IQueryResultItem()
{}


QueryResultItem::QueryResultItem(Base *parent) :
	TaskNodeItem(parent)
{}


QueryResultListItem::QueryResultListItem(Base *parent) :
	TaskNodeListItem(parent)
{}

IDM_PLUGIN_NS_END
