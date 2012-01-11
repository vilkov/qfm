#ifndef ARCNODEITEM_H_
#define ARCNODEITEM_H_

#include "../../arcplugin_ns.h"
#include "../../../../filesystem/tasks/items/filesystemtasknodeitemlist.h"


ARC_PLUGIN_NS_BEGIN

class IArcNodeItem
{
public:
	virtual ~IArcNodeItem();

	virtual bool isRoot() = 0;
	virtual bool isDir() = 0;
};


class ArcNodeItem : public TaskNodeItem, public IArcNodeItem
{
public:
	ArcNodeItem(Base *parent);
};


class ArcNodeListItem : public TaskNodeListItem, public IArcNodeItem
{
public:
	ArcNodeListItem(Base *parent);
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEITEM_H_ */
