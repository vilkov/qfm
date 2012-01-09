#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

IArcNodeItem::~IArcNodeItem()
{}

ArcNodeItem::ArcNodeItem(Base *parent) :
	FileSystemItem(parent)
{}

ArcNodeListItem::ArcNodeListItem(Base *parent) :
	FileSystemListItem(parent)
{}

ARC_PLUGIN_NS_END
