#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QSharedData>
#include "basetask.h"


FILE_SYSTEM_NS_BEGIN

class ControlableTask : public BaseTask
{
public:
	struct Params : public QSharedData, public BaseTask::Params
	{};

public:
	ControlableTask(Params *parameters) :
		m_params(parameters),
		m_controllerDead(false)
	{
		Q_ASSERT(m_params);
	}


protected:
	inline const volatile bool &isControllerDead() const { return m_controllerDead; }

protected:
	typedef QExplicitlySharedDataPointer<Params> ParamsPointer;
	inline Params *parameters() const { return m_params.data(); }
	const ParamsPointer &params() const { return m_params; }

private:
    ParamsPointer m_params;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* CONTROLABLETASK_H_ */
