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
#include "workthread.h"
#include "../exceptionshandler/exceptionshandler.h"
#include <QMutexLocker>


WorkThread::WorkThread(QObject *parent) :
	QThread(parent),
	m_stoped(true),
	m_abortRequested(false)
{}

void WorkThread::start(const Priority &priority)
{
    QMutexLocker locker(&m_mutex);

    if (m_stoped)
    {
	    m_stoped = false;

	    if (isRunning())
	    {
		    setPriority(priority);
	        m_condStoped.wakeOne();
	    }
	    else
	        QThread::start(priority);
    }
}

void WorkThread::stop()
{
    QMutexLocker locker(&m_mutex);

    if (!m_stoped)
    {
    	m_stoped = true;
		emit abortRequested();
    }
}

void WorkThread::stop(bool wait)
{
    QMutexLocker locker(&m_mutex);

	if (!m_stoped)
	{
		m_stoped = true;
		emit abortRequested();
	}

	if (wait)
		m_condStopAccepted.wait(&m_mutex);
}

void WorkThread::terminate()
{
    QMutexLocker locker(&m_mutex);

    if (!m_abortRequested)
    {
		m_abortRequested = true;

		if (m_stoped)
			m_condStoped.wakeOne();
		else
		{
			m_stoped = true;
			emit abortRequested();
		}
    }
}

void WorkThread::run()
{
	QMutexLocker locker(&m_mutex);

	forever
    {
		if (m_stoped)
		{
			m_condStopAccepted.wakeAll();
			m_condStoped.wait(&m_mutex);

			if (m_abortRequested)
				break;
		}
		else
		{
			m_mutex.unlock();
			TRY
			{
				execute();
			}
			CATCH_ALL
			(
				"WorkThread::run",
				DO_NOTHING
			)
			m_mutex.lock();

			if (m_abortRequested)
				break;
			else
			{
				m_stoped = true;
				emit done();
			}
		}
    }
}
