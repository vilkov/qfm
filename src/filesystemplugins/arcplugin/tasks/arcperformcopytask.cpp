#include "arcperformcopytask.h"


ARC_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(const QString &fileName, const ArcNodeItem * item, PScopedPointer<ICopyControl> &control, bool move, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName),
	m_item(item),
	m_control(control.take()),
	m_move(move)
{}

void PerformCopyTask::run(const volatile bool &aborted)
{

}

ARC_PLUGIN_NS_END
