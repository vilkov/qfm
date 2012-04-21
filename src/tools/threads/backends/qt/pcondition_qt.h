#ifndef PCONDITION_QT_H_
#define PCONDITION_QT_H_

#include <QtCore/QWaitCondition>
#include "pmutex_qt.h"


class PCondition
{
public:
	PCondition()
	{}

    void wait(PMutex &mutex) { m_condition.wait(&mutex.m_mutex); }
    void wakeOne() { m_condition.wakeOne(); }
    void wakeAll() { m_condition.wakeAll(); }

private:
    QWaitCondition m_condition;
};

#endif /* PCONDITION_QT_H_ */
