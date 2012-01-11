#ifndef ARCREADARCHIVETASK_H_
#define ARCREADARCHIVETASK_H_

#include "arctaskevent.h"
#include "../archive/arcarchive.h"


ARC_PLUGIN_NS_BEGIN

class ReadArchiveTask : public BaseTask
{
public:
	class Event : public TaskEvent
	{
	public:
		Event(BaseTask *task) :
			TaskEvent(ScanComplete)
		{}

		BaseTask *task;
		Archive::Contents contents;
	};

public:
	ReadArchiveTask(const QString &fileName, TasksNode *receiver);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCREADARCHIVETASK_H_ */
