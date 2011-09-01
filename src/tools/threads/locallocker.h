#ifndef LOCALLOCKER_H_
#define LOCALLOCKER_H_


#define DECLARE_LOCKER(Class)                      \
	class Locker                                   \
	{                                              \
	public:                                        \
		Locker(Class &instance) :                  \
			m_instance(instance),                  \
			m_locked(true)                         \
		{                                          \
			m_instance.lock();                     \
		}                                          \
		~Locker()                                  \
		{                                          \
			if (m_locked)                          \
				m_instance.unlock();               \
		}                                          \
                                                   \
		void lock()                                \
		{ m_instance.lock(); m_locked = true; }    \
		void unlock()                              \
		{ m_instance.unlock(); m_locked = false; } \
                                                   \
		const Class &instance() const              \
		{ return m_instance; }                     \
		                                           \
		Class &instance()                          \
		{ return m_instance; }                     \
                                                   \
	private:                                       \
		Locker(const Locker &other);               \
		void operator=(const Locker &other);       \
                                                   \
	private:                                       \
		Class &m_instance;                         \
		bool m_locked;                             \
	}

#endif /* LOCALLOCKER_H_ */
