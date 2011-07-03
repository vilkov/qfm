#ifndef PTHREAD_QT_H_
#define PTHREAD_QT_H_

#include <QtCore/QThread>
#include "../threadbase.h"


class PThread : public ThreadBase, protected QThread
{
public:
	PThread()
	{
		start();
	}

	void wait() { QThread::wait(); }
};

#endif /* PTHREAD_QT_H_ */
