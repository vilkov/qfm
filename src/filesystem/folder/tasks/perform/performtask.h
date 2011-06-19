#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../basetask.h"


FILE_SYSTEM_NS_BEGIN

template <typename BaseClass>
class PerformTask : public BaseClass
{
public:
	typedef BaseClass parent_class;

public:
	struct Params : public parent_class::Params
	{};

public:
	PerformTask(Params *params) :
		parent_class(params),
		m_canceled(false)
	{}
	PerformTask(Params *params, QObject *listener) :
		parent_class(params, listener),
		m_canceled(false)
	{}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
