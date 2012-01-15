#include "arcperformcopytask.h"
#include "../nodes/items/arcnodedirentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"


ARC_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(const QString &fileName, const ArcNodeItem::Base *item, PScopedPointer<ICopyControl> &control, bool move, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName),
	m_item(item),
	m_control(control.take()),
	m_move(move),
	m_overwriteAll(false),
	m_skipAllIfNotCopy(false)
{}

IFile::value_type *PerformCopyTask::buffer() const
{
	return const_cast<IFile::value_type *>(m_buffer);
}

IFile::size_type PerformCopyTask::bufferSize() const
{
	return FileReadWriteGranularity;
}

bool PerformCopyTask::overwriteAll() const
{
	return m_overwriteAll;
}

bool PerformCopyTask::skipAllIfNotCopy() const
{
	return m_skipAllIfNotCopy;
}

void PerformCopyTask::askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{

}

void PerformCopyTask::askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{

}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	Archive::State *state;
	PScopedPointer<Event> event(new Event(this, m_move));

	if (const Archive *archive = Archive::archive(m_fileName, &state))
	{
		archive->extract(state, m_item, m_control.data(), this, aborted);
		archive->endRead(state);
	}

	event->item = m_item;
	event->move = m_move;
	event->canceled = aborted;
	postEvent(event.take());
}

ARC_PLUGIN_NS_END
