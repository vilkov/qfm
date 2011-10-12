#ifndef FILESYSTEMLISTITEM_ITEM_H_
#define FILESYSTEMLISTITEM_ITEM_H_

#include "../../filesystemnode.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public ::Tools::Models::Tree::Item
{
public:
	FileSystemItem(Item *parent = 0);

	const Node::Holder &node() const { return m_node; }
	void setNode(const Node::Holder &node) { m_node = node; };
	void setNode(Node *node) { m_node = node; };

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_lockReason; }
	void lock(const QString &reason);
	void unlock();

private:
	Node::Holder m_node;
	bool m_locked;
	QString m_lockReason;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMLISTITEM_ITEM_H_ */
