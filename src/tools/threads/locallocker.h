#ifndef LOCALLOCKER_H_
#define LOCALLOCKER_H_

/**
 * This methods should be implemented
 * by the "Class" (could be in "private" section):
 *	void lock();
 *	bool try_lock();
 *	void unlock();
 *
 */

#define DECLARE_LOCKER(Class)                                      \
	class Locker                                                   \
	{                                                              \
	public:                                                        \
		template <int T> struct int_to_type {enum { value = T };}; \
		typedef int_to_type<0> Locked;                             \
		typedef int_to_type<1> Unlocked;                           \
		typedef int_to_type<2> TryLock;                            \
				                                                   \
	public:                                                        \
		Locker(Class &instance, const Locked &state = Locked()) :  \
			m_instance(instance),                                  \
			m_locked(true)                                         \
		{                                                          \
			m_instance.lock();                                     \
		}                                                          \
		Locker(Class *instance, const Locked &state = Locked()) :  \
			m_instance(*instance),                                 \
			m_locked(true)                                         \
		{                                                          \
			m_instance.lock();                                     \
		}                                                          \
		Locker(Class &instance, const Unlocked &state) :           \
			m_instance(instance),                                  \
			m_locked(false)                                        \
		{}                                                         \
		Locker(Class *instance, const Unlocked &state) :           \
			m_instance(*instance),                                 \
			m_locked(false)                                        \
		{}                                                         \
		Locker(Class &instance, const TryLock &state) :            \
			m_instance(instance),                                  \
			m_locked(m_instance.try_lock())                        \
		{}                                                         \
		Locker(Class *instance, const TryLock &state) :            \
			m_instance(*instance),                                 \
			m_locked(m_instance.try_lock())                        \
		{}                                                         \
		~Locker()                                                  \
		{                                                          \
			if (m_locked)                                          \
				m_instance.unlock();                               \
		}                                                          \
                                                                   \
        bool isLocked() const { return m_locked; }                 \
                                                                   \
		void lock()                                                \
		{ m_instance.lock(); m_locked = true; }                    \
		void unlock()                                              \
		{ m_instance.unlock(); m_locked = false; }                 \
                                                                   \
		const Class &instance() const                              \
		{ return m_instance; }                                     \
		                                                           \
		Class &instance()                                          \
		{ return m_instance; }                                     \
                                                                   \
	private:                                                       \
		Locker(const Locker &other);                               \
		void operator=(const Locker &other);                       \
                                                                   \
	private:                                                       \
		Class &m_instance;                                         \
		bool m_locked;                                             \
	};                                                             \
	friend class Locker

#endif /* LOCALLOCKER_H_ */
