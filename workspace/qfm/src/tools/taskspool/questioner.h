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
#ifndef QUESTIONER_H_
#define QUESTIONER_H_

#include "task.h"


TASKSPOOL_NS_BEGIN

template <typename T>
class Questioner
{
public:
	class Tristate
	{
	public:
		inline Tristate() :
			m_initialized(false)
		{}

		inline Tristate &operator=(bool value) { m_initialized = true; m_value = value; return *this; }

		inline operator bool() const { return m_value; }
		inline operator bool() const volatile { return m_value; }

		inline bool isInitialized() const { return m_initialized; }

	private:
		bool m_initialized;
		bool m_value;
	};

public:
	typedef T Object;
	typedef bool (Object::*Method)(const QString &error, Tristate &flag, const volatile Task::Flags &aborted);

public:
	inline Questioner(Object *object, Method method, const volatile Task::Flags &aborted) :
		m_object(object),
		m_method(method),
		m_aborted(aborted)
	{}

	template <typename F>
	inline bool askFor(F functor)
	{
		bool res;

		if (!(res = functor()))
			if (m_flag.isInitialized())
				return m_flag;
			else
			{
			    functor(m_error);
				res = (m_object->*m_method)(m_error, m_flag, m_aborted);
			}

		return res;
	}

private:
	Tristate m_flag;
	Object *m_object;
	Method m_method;
	QString m_error;
	const volatile Task::Flags &m_aborted;
};

TASKSPOOL_NS_END

#endif /* QUESTIONER_H_ */
