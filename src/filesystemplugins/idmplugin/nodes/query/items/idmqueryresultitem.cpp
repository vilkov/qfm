#include "idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultItem::QueryResultItem(Base *parent) :
	TaskNodeItem(parent)
{}


QueryResultListItem::QueryResultListItem(Base *parent) :
	QueryResultItem(parent)
{}

QueryResultListItem::Base *QueryResultListItem::at(size_type index) const
{
	return m_items.at(index);
}

QueryResultListItem::size_type QueryResultListItem::size() const
{
	return m_items.size();
}

QueryResultListItem::size_type QueryResultListItem::indexOf(Base *item) const
{
	return m_items.indexOf(static_cast<QueryResultItem *>(item));
}

IDM_PLUGIN_NS_END
