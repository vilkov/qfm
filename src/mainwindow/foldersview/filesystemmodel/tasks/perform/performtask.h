#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_


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
	{
		Q_ASSERT(params->source.object);
		Q_ASSERT(params->source.fileSystemTree);
		Q_ASSERT(params->source.entry);
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	volatile bool m_canceled;
};

#endif /* PERFORMTASK_H_ */
