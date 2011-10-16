#ifndef FILESYSTEMLISTITEM_ITEM_H_
#define FILESYSTEMLISTITEM_ITEM_H_

#include "../../filesystemnode.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public ::Tools::Models::Tree::Item
{
public:
	FileSystemItem(const Node::Holder &node, Item *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual bool isList() const;

	const Node::Holder &node() const { return m_node; }

private:
	Node::Holder m_node;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMLISTITEM_ITEM_H_ */
