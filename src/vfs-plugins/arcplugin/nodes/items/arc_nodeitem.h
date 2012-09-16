#ifndef ARC_NODEITEM_H_
#define ARC_NODEITEM_H_

#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../arc_ns.h"


ARC_PLUGIN_NS_BEGIN

class NodeItem : public TasksNodeItem
{
public:
	NodeItem(Base *parent);

	virtual void lock(const QString &reason);
	void unlock() { TasksNodeItem::unlock(); stop(); }

	virtual bool isRoot() const;

	void updateProgress(quint64 value, quint64 timeElapsed) { progress(value, timeElapsed); }
};

ARC_PLUGIN_NS_END

#endif /* ARC_NODEITEM_H_ */
