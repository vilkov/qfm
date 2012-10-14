/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TRYIER_H_
#define TRYIER_H_

#include "task.h"


TASKSPOOL_NS_BEGIN

template <typename T>
class Tryier
{
public:
	typedef T Object;
	typedef bool (Object::*Method)(const QString &error, bool &flag, const volatile Task::Flags &aborted);

public:
	inline Tryier(Object *object, Method method, const volatile Task::Flags &aborted) :
		m_flag(false),
		object(object),
		m_method(method),
		m_aborted(aborted)
	{}

	template <typename F>
	inline bool tryTo(F functor)
	{
		bool res;

		do
			if (res = functor(m_error))
				break;
			else
				if (m_flag)
					break;
				else
					res = (object->*m_method)(m_error, m_flag, m_aborted);
		while (res && !m_aborted);

		return res;
	}

private:
	bool m_flag;
	Object *object;
	Method m_method;
	QString m_error;
	const volatile Task::Flags &m_aborted;
};

TASKSPOOL_NS_END

#endif /* TRYIER_H_ */
