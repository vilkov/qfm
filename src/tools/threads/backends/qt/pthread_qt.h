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

	virtual void start() { m_holder.start(Holder::LowPriority); }
	virtual void terminate() { m_holder.terminate(); }

	void wait() { m_holder.wait(); }

private:
    class Holder : public QThread
    {
    public:
    	Holder(PThread *thread) :
    		QThread(),
        	m_thread(thread)
    	{}

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
