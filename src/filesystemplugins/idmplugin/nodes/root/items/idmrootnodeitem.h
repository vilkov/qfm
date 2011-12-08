#ifndef IDMROOTNODEITEM_H_
#define IDMROOTNODEITEM_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/items/filesystemtasknodeitemlist.h"


IDM_PLUGIN_NS_BEGIN

class IRootNodeItem
{
public:
	virtual ~IRootNodeItem();

	virtual bool isRoot() = 0;
	virtual bool isFiles() = 0;
	virtual bool isEntity() = 0;
};


class RootNodeItem : public TaskNodeItem, public IRootNodeItem
{
public:
	RootNodeItem(Base *parent);
};


class RootNodeListItem : public TaskNodeListItem, public IRootNodeItem
{
public:
	RootNodeListItem(Base *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEITEM_H_ */
