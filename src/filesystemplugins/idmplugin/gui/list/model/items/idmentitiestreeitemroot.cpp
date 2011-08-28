#include "idmentitiestreeitemroot.h"


IDM_PLUGIN_NS_BEGIN

IdmEntitiesTreeItemRoot::IdmEntitiesTreeItemRoot(IdmItem *parent) :
	IdmEntitiesTreeItem(0, parent)
{}

bool IdmEntitiesTreeItemRoot::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
