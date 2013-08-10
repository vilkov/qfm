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
#include "threads_workthread.h"
#include "../exceptionshandler/exceptionshandler.h"


THREADS_NS_BEGIN

WorkThread::WorkThread() :
	Thread(),
	m_stoped(true),
	m_abortRequested(false)
{}

void WorkThread::start(Priority priority)
{
    Mutex::Locker lock(m_mutex);

    if (m_stoped)
    {
	    m_stoped = false;

	    if (isRunning())
	    {
		    setPriority(priority);
	        m_condStoped.wakeOne();
	    }
	    else
	        Thread::start(priority);
    }
}

void WorkThread::stop()
{
    Mutex::Locker lock(m_mutex);

    if (!m_stoped)
    	m_stoped = true;
}

void WorkThread::stop(bool wait)
{
    Mutex::Locker lock(m_mutex);

	if (!m_stoped)
		m_stoped = true;

	if (wait)
		m_condStopAccepted.wait(m_mutex);
}

void WorkThread::terminate()
{
    Mutex::Locker lock(m_mutex);

    if (!m_abortRequested)
    {
		m_abortRequested = true;

		if (m_stoped)
			m_condStoped.wakeOne();
		else
			m_stoped = true;
    }
}

void WorkThread::run()
{
	Mutex::Locker lock(m_mutex);

	forever
    {
		if (m_stoped)
		{
			m_condStopAccepted.wakeAll();
			m_condStoped.wait(m_mutex);

			if (m_abortRequested)
				break;
		}
		else
		{
			TRY
			{
			    Mutex::Unlocker unlock(lock);
				execute();
			}
			CATCH_ALL
			(
				"WorkThread::run",
				DO_NOTHING
			)

			if (m_abortRequested)
				break;
			else
				m_stoped = true;
		}
    }
}

THREADS_NS_END
