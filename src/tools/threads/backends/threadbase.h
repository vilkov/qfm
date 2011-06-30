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

protected:
	virtual void run() = 0;
};

#endif /* THREADBASE_H_ */
