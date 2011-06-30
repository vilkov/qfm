#ifndef PCONDITION_QT_H_
#define PCONDITION_QT_H_

#include "pmutex_qt.h"


class PCondition
{
public:
	PCondition()
	{}

    void wait(PMutex &mutex) {}
    void wakeOne() {}
    void wakeAll() {}

private:
};

#endif /* PCONDITION_QT_H_ */
