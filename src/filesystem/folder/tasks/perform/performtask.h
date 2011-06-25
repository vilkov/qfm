#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public DestControlableTask
{
public:
	typedef DestControlableTask::Params Params;

public:
	PerformTask(Params *params, QObject *listener) :
		DestControlableTask(params, listener),
		m_canceled(false)
	{}

protected:
	inline Params *parameters() const { return static_cast<Params*>(DestControlableTask::parameters()); }

protected:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
