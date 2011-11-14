#include "idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

IQueryResultItem::~IQueryResultItem()
{}


QueryResultItem::QueryResultItem(IdmItem *parent) :
	IdmItem(parent)
{}


QueryResultListItem::QueryResultListItem(IdmItem *parent) :
	IdmListItem(parent)
{}

IDM_PLUGIN_NS_END
