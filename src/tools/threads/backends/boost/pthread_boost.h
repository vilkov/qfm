#ifndef PTHREAD_BOOST_H_
#define PTHREAD_BOOST_H_

#include <boost/thread/thread.hpp>
#include "../threadbase.h"


class PThread : public ThreadBase
{
public:
	PThread() :
		m_thread(boost::bind(&PThread::run, this))
	{}

	void wait() { m_thread.join(); }

private:
	boost::thread m_thread;
};

#endif /* PTHREAD_BOOST_H_ */
