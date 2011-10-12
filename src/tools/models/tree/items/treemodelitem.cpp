#include "treemodelitem.h"


MODELS_TREE_NS_BEGIN

Item::Item(Item *parent) :
	m_parent(parent)
{}

Item::~Item()
{}

MODELS_TREE_NS_END
