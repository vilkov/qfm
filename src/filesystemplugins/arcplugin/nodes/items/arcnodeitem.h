#ifndef ARCNODEITEM_H_
#define ARCNODEITEM_H_

#include "../../arcplugin_ns.h"
#include "../../../../filesystem/model/items/filesystemlistitem.h"


ARC_PLUGIN_NS_BEGIN

class IArcNodeItem
{
public:
	virtual ~IArcNodeItem();

	virtual bool isRoot() = 0;
	virtual bool isDir() = 0;
};


class ArcNodeItem : public FileSystemItem, public IArcNodeItem
{
public:
	ArcNodeItem(Base *parent);
};


class ArcNodeListItem : public FileSystemListItem, public IArcNodeItem
{
public:
	ArcNodeListItem(Base *parent);
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEITEM_H_ */
