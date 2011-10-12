#ifndef TREELISTITEM_H_
#define TREELISTITEM_H_

#include <QtCore/QList>
#include "treeitem.h"


MODELS_NS_BEGIN

class TreeListItem : public TreeItem
{
public:
	typedef QList<TreeItem*>      Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	TreeListItem(TreeItem *parent = 0);
	virtual ~TreeListItem();

	/* TreeItem */
	virtual bool isList() const;

	TreeItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(TreeItem *item) const { return m_items.indexOf(item); }

protected:
	Container m_items;
};

MODELS_NS_END

#endif /* TREELISTITEM_H_ */
