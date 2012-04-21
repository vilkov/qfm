#ifndef TREEMODELCONTAINER_H_
#define TREEMODELCONTAINER_H_

#include "items/treemodelitem.h"


MODELS_TREE_NS_BEGIN

class Container
{
public:
	typedef ::Tools::Models::Tree::Item Item;
	typedef Item::size_type             size_type;
	enum { InvalidIndex = Item::InvalidIndex };

public:
	virtual ~Container();

	virtual size_type size() const = 0;
	virtual Item *at(size_type index) const = 0;
	virtual size_type indexOf(Item *item) const = 0;
};

MODELS_TREE_NS_END

#endif /* TREEMODELCONTAINER_H_ */
