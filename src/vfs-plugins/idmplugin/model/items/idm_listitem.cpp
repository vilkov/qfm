#include "idm_listitem.h"


IDM_PLUGIN_NS_BEGIN

IdmListItem::IdmListItem(Base *parent) :
	IdmItem(parent)
{}

IdmListItem::Base *IdmListItem::at(size_type index) const
{
	return m_items.at(index);
}

IdmListItem::size_type IdmListItem::size() const
{
	return m_items.size();
}

IdmListItem::size_type IdmListItem::indexOf(Base *item) const
{
	return m_items.indexOf(static_cast<IdmItem *>(item));
}

IDM_PLUGIN_NS_END
