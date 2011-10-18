#ifndef FILESYSTEMLISTITEM_ITEM_H_
#define FILESYSTEMLISTITEM_ITEM_H_

#include "../../filesystemnode.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public ::Tools::Models::Tree::Item
{
public:
	FileSystemItem(Item *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual bool isList() const;

	Node *node() const { return m_node; }

protected:
	void setNode(Node *node) { m_node = node; }

private:
	Node *m_node;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMLISTITEM_ITEM_H_ */
