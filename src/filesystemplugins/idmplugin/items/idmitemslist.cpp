#include "idmitemslist.h"


IDM_PLUGIN_NS_BEGIN

IdmItemsList::IdmItemsList(IdmItem *parent) :
	IdmItem(parent)
{}

IdmItemsList::~IdmItemsList()
{
	qDeleteAll(m_items);
}

bool IdmItemsList::isRoot() const
{
	return false;
}

bool IdmItemsList::isList() const
{
	return true;
}

bool IdmItemsList::isMenuItem() const
{
	return false;
}

bool IdmItemsList::isValueItem() const
{
	return false;
}

bool IdmItemsList::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
