#ifndef ARCNODEITEM_H_
#define ARCNODEITEM_H_

#include "../../arcplugin_ns.h"
#include "../../../../filesystem/tasks/model/items/filesystemtasksnodeitem.h"


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

#endif /* ARCNODEITEM_H_ */
