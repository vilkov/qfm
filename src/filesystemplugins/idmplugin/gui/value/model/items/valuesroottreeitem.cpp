#include "valuesroottreeitem.h"


IDM_PLUGIN_NS_BEGIN

ValuesRootTreeItem::ValuesRootTreeItem(IdmItem *parent) :
	ValuesTreeItem(0, parent)
{}

bool ValuesRootTreeItem::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
