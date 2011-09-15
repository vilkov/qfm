#include "valuelistrootitem.h"


IDM_PLUGIN_NS_BEGIN

ValueListRootItem::ValueListRootItem(IdmItem *parent) :
	ValueListItem(0, QString(), parent)
{}

bool ValueListRootItem::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
