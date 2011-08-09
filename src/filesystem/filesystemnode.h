#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QSet>
#include <QtCore/QAbstractItemModel>
#include "filesystem_ns.h"
#include "interfaces/filesysteminode.h"


FILE_SYSTEM_NS_BEGIN

class Node : public QAbstractItemModel, public INode
{
	Q_DISABLE_COPY(Node)

public:
	Node(Node *parent = 0);

	/* INode */
	virtual INode *root() const;
	virtual int columnsCount() const;

	const QModelIndex &parentEntryIndex() const { return m_parentEntryIndex; }
	void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

	virtual void switchTo(Node *node, const QModelIndex &selected) = 0;

protected:
	bool isVisible() const { return !m_view.isEmpty(); }
	void addView(INodeView *view) { m_view.insert(view); }
	void removeView(INodeView *view) { m_view.remove(view); }

private:
	typedef QSet<INodeView*> ViewSet;

private:
	ViewSet m_view;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
