#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include "../../../model/items/idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

class IQueryResultItem
{
public:
	virtual ~IQueryResultItem();

	virtual bool isRoot() = 0;
	virtual bool isProperty() = 0;
	virtual bool isValue() = 0;
};


class QueryResultItem : public IdmItem, public IQueryResultItem
{
public:
	QueryResultItem(IdmItem *parent);
};


class QueryResultListItem : public IdmListItem, public IQueryResultItem
{
public:
	QueryResultListItem(IdmItem *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */
