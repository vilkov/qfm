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
	Archive::State *state;
	PScopedPointer<Event> event(new Event(this, m_move));

	if (const Archive *archive = Archive::archive(m_fileName, &state))
	{
		archive->extract(state, m_item, m_control.data(), m_buffer, FileReadWriteGranularity, aborted);
		archive->endRead(state);
	}

	event->item = m_item;
	event->move = m_move;
	event->canceled = aborted;
	postEvent(event.take());
}

ARC_PLUGIN_NS_END
