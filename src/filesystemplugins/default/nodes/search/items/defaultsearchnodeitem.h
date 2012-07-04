#ifndef DEFAULTSEARCHNODEITEM_H_
#define DEFAULTSEARCHNODEITEM_H_

#include "../../../model/items/defaultnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchNodeItem : public NodeItem
{
public:
	typedef QList<Holder> Container;

public:
	SearchNodeItem(WrappedNodeItem *item, Base *parent = 0);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

private:
	Container m_items;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHNODEITEM_H_ */
