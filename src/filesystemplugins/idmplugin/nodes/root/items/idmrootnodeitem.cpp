#include "idmrootnodeitem.h"


IDM_PLUGIN_NS_BEGIN

IRootNodeItem::~IRootNodeItem()
{}

RootNodeItem::RootNodeItem(Base *parent) :
	TaskNodeItem(parent)
{}

RootNodeListItem::RootNodeListItem(Base *parent) :
	TaskNodeListItem(parent)
{}

IDM_PLUGIN_NS_END
