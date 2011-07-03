#ifndef PTHREAD_QT_H_
#define PTHREAD_QT_H_

#include <QtCore/QThread>
#include "../threadbase.h"


class PThread : public ThreadBase
{
public:
	PThread() :
		m_holder(this)
	{}

    void wait() { m_holder.wait(); }

private:
    class Holder : public QThread
    {
    public:
    	Holder(PThread *thread) :
    		QThread(),
        	m_thread(thread)
    	{
    		start(LowPriority);
    	}

    protected:
        virtual void run() { m_thread->run(); }

    private:
    	PThread *m_thread;
    };
    friend class Holder;

private:
    Holder m_holder;
};

#endif /* PTHREAD_QT_H_ */
