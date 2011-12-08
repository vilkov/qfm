#include "idmrootnodeentitybaseitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeEntityBaseItem::RootNodeEntityBaseItem(Base *parent) :
	RootNodeItem(parent)
{}

bool RootNodeEntityBaseItem::isRoot()
{
	return false;
}

bool RootNodeEntityBaseItem::isFiles()
{
	return false;
}

bool RootNodeEntityBaseItem::isEntity()
{
	return true;
}


RootNodeEntityBaseListItem::RootNodeEntityBaseListItem(Base *parent) :
	RootNodeListItem(parent)
{}

bool RootNodeEntityBaseListItem::isRoot()
{
	return false;
}

bool RootNodeEntityBaseListItem::isFiles()
{
	return false;
}

bool RootNodeEntityBaseListItem::isEntity()
{
	return true;
}

IDM_PLUGIN_NS_END
