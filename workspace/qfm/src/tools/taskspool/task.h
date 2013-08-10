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
#ifndef TASK_H_
#define TASK_H_

#include "taskspool_ns.h"
#include <limits.h>


TASKSPOOL_NS_BEGIN

class Task
{
public:
	class Bit;
	class StaticFlag;


	class Flags
	{
	public:
		typedef unsigned int value_type;

	public:
		Flags() :
			m_value(0)
		{}

		operator volatile bool() const volatile { return m_value; }
		operator volatile bool() const { return m_value; }

		volatile bool operator!() const volatile { return !m_value; }
		volatile bool operator!() const { return !m_value; }

	private:
		Flags(const Flags &other);
		Flags &operator=(const Flags &other);

	private:
		friend class Bit;
		friend class StaticFlag;
		bool testBit(int i) const { return m_value & (1 << i); }
		void setBit(int i) { m_value |= (1 << i); }
		void clearBit(int i) { m_value &= ~(1 << i); }
		void setBit(int i, bool value) { m_value = (m_value & ((~0U << i) << 1)) | (value << i) | (m_value & ((~0U >> (sizeof(value_type) * CHAR_BIT - i - 1)) >> 1)); }

	private:
		value_type m_value;
	};


	class Flag
	{
	public:
		Flag() :
			m_value(false),
			m_bit(0)
		{}

		operator volatile bool() const volatile { return m_value; }
		operator volatile bool() const { return m_value; }

		volatile bool operator!() const volatile { return !m_value; }
		volatile bool operator!() const { return !m_value; }

		inline void operator=(bool value);

	private:
		Flag(const Flag &other);
		Flag &operator=(const Flag &other);

	private:
		friend class Bit;
		bool m_value;
		Bit *m_bit;
	};


	class StaticFlag
	{
	public:
		StaticFlag(int bit, Flags &flags) :
			m_value(false),
			m_bit(bit),
			m_flags(flags)
		{}

		operator volatile bool() const volatile { return m_value; }
		operator volatile bool() const { return m_value; }

		volatile bool operator!() const volatile { return !m_value; }
		volatile bool operator!() const { return !m_value; }

		inline void operator=(bool value);

	private:
		StaticFlag(const Flag &other);
		StaticFlag &operator=(const Flag &other);

	private:
		bool m_value;
		int m_bit;
		Flags &m_flags;
	};


	class Bit
	{
	public:
		Bit(Flag &flag, int bit, Flags &flags) :
			m_flag(flag),
			m_bit(bit),
			m_flags(flags)
		{
			m_flag.m_bit = this;
		}
		~Bit()
		{
			m_flag.m_bit = 0;
		}

	private:
		Bit(const Bit &other);
		Bit &operator=(const Bit &other);

	private:
		friend class Flag;
		void set() { m_flags.setBit(m_bit); }
		void clear() { m_flags.clearBit(m_bit); }
		void operator=(bool value) { m_flags.setBit(m_bit, value); }

	private:
		Flag &m_flag;
		int m_bit;
		Flags &m_flags;
	};


public:
	Task();
	virtual ~Task();

protected:
	friend class TaskThread;
	Flags &flags() { return m_flags; }
	virtual void run(const volatile Flags &aborted) = 0;

private:
	Task(const Task &other);
	Task &operator=(const Task &other);

private:
	Flags m_flags;
};


void Task::Flag::operator=(bool value)
{
	if (m_bit)
		(*m_bit) = m_value = value;
	else
		m_value = value;
}


void Task::StaticFlag::operator=(bool value)
{
	m_flags.setBit(m_bit, m_value = value);
}

TASKSPOOL_NS_END

#endif /* TASK_H_ */
