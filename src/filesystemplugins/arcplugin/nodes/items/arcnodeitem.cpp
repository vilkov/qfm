#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

IArcNodeItem::~IArcNodeItem()
{}

ArcNodeItem::ArcNodeItem(Base *parent) :
	TaskNodeItem(parent)
{}

ArcNodeListItem::ArcNodeListItem(Base *parent) :
	TaskNodeListItem(parent)
{}

ARC_PLUGIN_NS_END
