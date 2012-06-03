#ifndef TRYIER_H_
#define TRYIER_H_

#include "task.h"


TASKSPOOL_NS_BEGIN

template <typename T, int N>
class Tryier
{
public:
	typedef T Object;
	typedef bool (Object::*Method)(const QString &error, bool &flag, const volatile Task::Flags &aborted);

public:
	inline Tryier(Object *object, const volatile Task::Flags &aborted) :
		object(object),
		m_aborted(aborted)
	{}

	template <int I>
	inline void setFlag(Method method)
	{
		m_flags[I].value = false;
		m_flags[I].method = method;
	}

	template <int I, typename F>
	inline bool tryTo(F functor)
	{
		bool res;

		do
			if (res = functor(m_error))
				break;
			else
				if (m_flags[I].value)
					break;
				else
					res = (object->*m_flags[I].method)(m_error, m_flags[I].value, m_aborted);
		while (res && !m_aborted);

		return res;
	}

private:
	struct Flag
	{
		bool value;
		Method method;
	};

private:
	Object *object;
	Flag m_flags[N];
	QString m_error;
	const volatile Task::Flags &m_aborted;
};

TASKSPOOL_NS_END

#endif /* TRYIER_H_ */
