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

	virtual void start() = 0;
	virtual void terminate() = 0;

protected:
	virtual void run() = 0;
};

#endif /* THREADBASE_H_ */
