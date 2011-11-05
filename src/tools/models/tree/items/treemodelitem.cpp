#include "treemodelitem.h"


MODELS_TREE_NS_BEGIN

Item::Item(Item *parent) :
	m_parent(parent)
{}

Item::~Item()
{}

bool Item::isList() const
{
	return false;
}

MODELS_TREE_NS_END
