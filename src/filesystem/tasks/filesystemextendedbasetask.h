#ifndef FILESYSTEMEXTENDEDBASETASK_H_
#define FILESYSTEMEXTENDEDBASETASK_H_

#include "filesystembasetask.h"
#include "../interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

class ExtendedBaseTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
		Q_DISABLE_COPY(Event)

	public:
		ICopyControl::Holder destination;

	protected:
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination) :
			BaseTask::Event(task, type),
			destination(destination.take())
		{}
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled) :
			BaseTask::Event(task, type, canceled),
			destination(destination.take())
		{}
	};

public:
	ExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);

	const ICopyControl::Holder &destination() const { return m_destination; }

protected:
	ICopyControl::Holder &destination() { return m_destination; }

private:
    ICopyControl::Holder m_destination;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMEXTENDEDBASETASK_H_ */
