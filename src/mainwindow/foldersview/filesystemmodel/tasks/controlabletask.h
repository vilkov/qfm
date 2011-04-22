#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QSharedData>
#include "../events/filesystemmodelevents.h"


class ControlableTask : public FileSystemModelEvents::BaseTask
{
public:
	struct Params : public QSharedData, public FileSystemModelEvents::BaseTask::Params
	{};

public:
	ControlableTask(Params *parameters);

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

#endif /* CONTROLABLETASK_H_ */
