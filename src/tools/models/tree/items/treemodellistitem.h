#ifndef TREEMODELLISTITEM_H_
#define TREEMODELLISTITEM_H_

#include <QtCore/QList>
#include "treemodelitem.h"


MODELS_TREE_NS_BEGIN

class ListItem : public Item
{
public:
	typedef QList<Base*>         Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	ListItem(Base *parent = 0);
	virtual ~ListItem();

	/* Item */
	virtual bool isList() const;

	Base *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(Base *item) const { return m_items.indexOf(item); }

protected:
	Container m_items;
};

MODELS_TREE_NS_END

#endif /* TREEMODELLISTITEM_H_ */
