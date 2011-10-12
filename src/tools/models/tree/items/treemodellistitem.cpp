#include "treemodellistitem.h"


MODELS_TREE_NS_BEGIN

ListItem::ListItem(Item *parent) :
	Item(parent)
{}

ListItem::~ListItem()
{
	qDeleteAll(m_items);
}

bool ListItem::isList() const
{
	return true;
}

MODELS_TREE_NS_END
