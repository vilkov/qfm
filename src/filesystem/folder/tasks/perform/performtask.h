#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../../../filesystem_ns.h"
#include "../../items/filesystemfoldernodeitemlist.h"


FILE_SYSTEM_NS_BEGIN

template <typename BaseClass>
class PerformTask : public BaseClass
{
public:
	typedef BaseClass parent_class;

public:
	struct Params : public parent_class::Params
	{
		typename parent_class::Params::Snapshot source;
	};

public:
	PerformTask(Params *params) :
		parent_class(params),
		m_canceled(false)
	{}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
