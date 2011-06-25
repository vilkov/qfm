#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public DestControlableTask
{
public:
	PerformTask(QObject *receiver) :
		DestControlableTask(receiver),
		m_canceled(false)
	{}

protected:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
