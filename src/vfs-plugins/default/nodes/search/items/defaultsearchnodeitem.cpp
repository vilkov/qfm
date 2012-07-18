#include "defaultsearchnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchNodeItem::SearchNodeItem(SnapshotItem *item, Base *parent) :
	NodeItem(item->info(), parent)
{
	setTotalSize(item->totalSize());

	for (SnapshotItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		m_items.push_back(Holder(new SearchNodeItem((*i), this)));
}

SearchNodeItem::Base *SearchNodeItem::at(size_type index) const
{
	return m_items.at(index).data();
}

SearchNodeItem::size_type SearchNodeItem::size() const
{
	return m_items.size();
}

SearchNodeItem::size_type SearchNodeItem::indexOf(Base *item) const
{
	Holder holder(static_cast<SearchNodeItem *>(item));
	return m_items.indexOf(holder);
}

DEFAULT_PLUGIN_NS_END
