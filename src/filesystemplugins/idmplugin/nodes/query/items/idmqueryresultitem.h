#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultItem : public TasksNodeItem
{
public:
	QueryResultItem(Base *parent);

	virtual bool isRoot() = 0;
	virtual bool isProperty() = 0;
	virtual bool isValue() = 0;
	virtual bool isPath() = 0;
	virtual bool isRootPathValue() = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */
