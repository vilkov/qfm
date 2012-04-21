#ifndef PCONDITION_BOOST_H_
#define PCONDITION_BOOST_H_

#include <boost/thread/condition.hpp>
#include "pmutex_boost.h"


class PCondition
{
public:
	PCondition()
	{}

    void wait(PMutex &mutex) { m_condition.wait(mutex); }
    void wakeOne() { m_condition.notify_one(); }
    void wakeAll() { m_condition.notify_all(); }

private:
    boost::condition m_condition;
};

#endif /* PCONDITION_BOOST_H_ */
