#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/items/filesystemtasknodeitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultItem : public TaskNodeItem
{
public:
	QueryResultItem(Base *parent);

	virtual bool isRoot() = 0;
	virtual bool isProperty() = 0;
	virtual bool isValue() = 0;
};


class QueryResultListItem : public QueryResultItem
{
public:
	typedef QList<QueryResultItem *> Container;

public:
	QueryResultListItem(Base *parent);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

protected:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */
