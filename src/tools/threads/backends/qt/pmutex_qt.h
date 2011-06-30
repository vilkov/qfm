#ifndef PMUTEX_QT_H_
#define PMUTEX_QT_H_


class PMutex
{
public:
	PMutex()
	{}

	void lock() {}
	void unlock() {}

private:
	friend class PCondition;
	friend class PMutexLocker;
};


class PMutexLocker
{
public:
	PMutexLocker(PMutex &mutex)
	{}

	void lock() {}
	void unlock() {}

private:
};

#endif /* PMUTEX_QT_H_ */
