#ifndef THREADBASE_H_
#define THREADBASE_H_


class ThreadBase
{
public:
	enum Priority
	{
		Low,
		Normal,
		High
	};

public:
	virtual ~ThreadBase() {}
};

#endif /* THREADBASE_H_ */
