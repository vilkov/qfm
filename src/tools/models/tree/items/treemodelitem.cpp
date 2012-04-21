#include "treemodelitem.h"


MODELS_TREE_NS_BEGIN

Item::Item(Item *parent) :
	m_parent(parent)
{}

Item::~Item()
{}

Item::Base *Item::at(size_type index) const
{
	return NULL;
}

Item::size_type Item::size() const
{
	return 0;
}

Item::size_type Item::indexOf(Base *item) const
{
	return InvalidIndex;
}

MODELS_TREE_NS_END
