#ifndef ARCPERFORMCOPYTASK_H_
#define ARCPERFORMCOPYTASK_H_

#include "arctaskevent.h"


ARC_PLUGIN_NS_BEGIN

class PerformCopyTask : public BaseTask
{
public:
	class Event : public TaskEvent
	{
	public:
		Event() :
			TaskEvent(CopyComplete)
		{}
	};

public:
	PerformCopyTask(const QString &fileName, TasksNode *receiver);

protected:
	virtual void run(const volatile bool &aborted);

private:
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCPERFORMCOPYTASK_H_ */
