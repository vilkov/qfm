#include "idmrootnodeitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeItem::RootNodeItem(Base *parent) :
	TasksNodeItem(parent)
{}

bool RootNodeItem::isRoot()
{
	return false;
}

bool RootNodeItem::isFiles()
{
	return false;
}

bool RootNodeItem::isEntity()
{
	return false;
}

bool RootNodeItem::isProperty()
{
	return false;
}

RootNodeListItem::RootNodeListItem(Base *parent) :
	RootNodeItem(parent)
{}

RootNodeListItem::~RootNodeListItem()
{
	qDeleteAll(m_items);
}

RootNodeListItem::Base *RootNodeListItem::at(size_type index) const
{
	return m_items.at(index);
}

RootNodeListItem::size_type RootNodeListItem::size() const
{
	return m_items.size();
}

RootNodeListItem::size_type RootNodeListItem::indexOf(Base *item) const
{
	return m_items.indexOf(static_cast<RootNodeItem *>(item));
}

IDM_PLUGIN_NS_END
