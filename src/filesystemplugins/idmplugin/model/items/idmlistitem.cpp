#include "idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

IdmListItem::IdmListItem(IdmItem *parent) :
	IdmItem(parent)
{}

IdmListItem::~IdmListItem()
{
	qDeleteAll(m_items);
}

bool IdmListItem::isList() const
{
	return true;
}

IDM_PLUGIN_NS_END
