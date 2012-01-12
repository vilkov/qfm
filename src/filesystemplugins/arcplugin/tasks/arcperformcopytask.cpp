#include "arcperformcopytask.h"
#include "../archive/arcarchive.h"
#include "../nodes/items/arcnodedirentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"


ARC_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(const QString &fileName, const ArcNodeItem::Base *item, PScopedPointer<ICopyControl> &control, bool move, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName),
	m_item(item),
	m_control(control.take()),
	m_move(move)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{


//	if (val)
//	{
//
//		PScopedPointer<Event> event(new Event(this, m_move));
//
//		event->canceled = isCanceled();
//
//		if (!aborted && !isReceiverDead())
//			postEvent(event.take());
//	}
}

ARC_PLUGIN_NS_END
