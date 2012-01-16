#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeItem::ArcNodeItem(Base *parent) :
	TaskNodeItem(parent)
{}

ArcNodeListItem::ArcNodeListItem(Base *parent) :
	ArcNodeItem(parent)
{}

ArcNodeListItem::Base *ArcNodeListItem::at(size_type index) const
{
	return m_items.at(index);
}

ArcNodeListItem::size_type ArcNodeListItem::size() const
{
	return m_items.size();
}

ArcNodeListItem::size_type ArcNodeListItem::indexOf(Base *item) const
{
	return m_items.indexOf(item);
}

ARC_PLUGIN_NS_END
