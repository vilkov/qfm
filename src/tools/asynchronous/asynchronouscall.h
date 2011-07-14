#ifndef ASYNCHRONOUSCALL_H_
#define ASYNCHRONOUSCALL_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>


class AsynchronousCall
{
public:
	AsynchronousCall();
	~AsynchronousCall();

	void wait();
	bool wait(unsigned int timeout);
	template <typename Functor> inline void perform(Functor f);

private:
	void run();

private:
	class Functor
	{
	public:
		virtual ~Functor() {}
		virtual void call() = 0;
	};

	template <typename T>
	class Callable : public Functor
	{
	public:
		Callable(const T &functor) :
			m_functor(functor)
		{}

		virtual void call() { m_functor(); }

	private:
		T m_functor;
	};

private:
	bool m_busy;
	bool m_aborted;
	boost::thread m_thread;
	boost::mutex m_mutex;
	boost::condition_variable m_condition;
	boost::condition_variable m_completeCondition;
	boost::shared_ptr<Functor> m_callable;
};


AsynchronousCall::AsynchronousCall() :
	m_busy(false),
	m_aborted(false),
	m_thread(boost::bind(&AsynchronousCall::run, this))
{}

AsynchronousCall::~AsynchronousCall()
{
	boost::unique_lock<boost::mutex> locker(m_mutex);
	m_aborted = true;
	m_condition.notify_one();
	locker.unlock();
	m_thread.join();
}

void AsynchronousCall::wait()
{
	boost::unique_lock<boost::mutex> locker(m_mutex);

	if (m_busy)
		m_completeCondition.wait(m_mutex);
}

bool AsynchronousCall::wait(unsigned int timeout)
{
	boost::unique_lock<boost::mutex> locker(m_mutex);

	if (m_busy)
		return m_completeCondition.timed_wait(m_mutex, boost::get_system_time() + boost::posix_time::milliseconds(timeout));
}

template <typename Functor>
inline void AsynchronousCall::perform(Functor f)
{
	boost::unique_lock<boost::mutex> locker(m_mutex);
	m_callable.reset(new Callable<Functor>(f));
	m_condition.notify_one();
}

void AsynchronousCall::run()
{
	boost::unique_lock<boost::mutex> locker(m_mutex);

    for (;;)
    {
        if (m_aborted)
            return;
        else
			if (!m_callable)
				m_condition.wait(m_mutex);
			else
			{
				boost::scoped_ptr<Functor> callable(m_callable);
				m_callable.reset();

				m_busy = true;
				locker.unlock();
				try
				{
					callable->call();
				}
				catch (...)
				{}
				locker.lock();
				m_busy = false;
				m_completeCondition.notify_all();
			}
    }
}

#endif /* ASYNCHRONOUSCALL_H_ */
