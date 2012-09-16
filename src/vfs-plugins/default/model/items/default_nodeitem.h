#ifndef DEFAULT_NODEITEM_H_
#define DEFAULT_NODEITEM_H_

#include <vfs/vfs_node.h>
#include <vfs/interfaces/vfs_ifileinfo.h>
#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class NodeItem : public TasksNodeItem
{
public:
	NodeItem(IFileInfo::Holder &info, Base *parent = 0);
	NodeItem(IFileInfo::Holder &info, Node *node, Base *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	virtual bool isRootItem() const;

	const IFileInfo::Holder &info() const { return m_info; }

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

	void update(IFileInfo::Holder &info) { m_info = info.take(); }

	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

public:
	const QVariant &totalSize() const { return m_totalSize; }
	void setTotalSize(quint64 value) { m_totalSize = value; }
	void clearTotalSize() { m_totalSize.clear(); }

	void updateProgress(quint64 value, quint64 timeElapsed) { progress(value, timeElapsed); }

private:
	Node *m_node;
	QVariant m_totalSize;
	IFileInfo::Holder m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_NODEITEM_H_ */
