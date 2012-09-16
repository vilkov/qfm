#ifndef IDM_QUERYRESULTITEM_H_
#define IDM_QUERYRESULTITEM_H_

#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../../idm_ns.h"


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

#endif /* IDM_QUERYRESULTITEM_H_ */
