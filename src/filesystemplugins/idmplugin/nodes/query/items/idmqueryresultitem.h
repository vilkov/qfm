#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/items/filesystemtasknodeitemlist.h"


IDM_PLUGIN_NS_BEGIN

class IQueryResultItem
{
public:
	virtual ~IQueryResultItem();

	virtual bool isRoot() = 0;
	virtual bool isProperty() = 0;
	virtual bool isValue() = 0;
};


class QueryResultItem : public TaskNodeItem, public IQueryResultItem
{
public:
	QueryResultItem(Base *parent);
};


class QueryResultListItem : public TaskNodeListItem, public IQueryResultItem
{
public:
	QueryResultListItem(Base *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */
