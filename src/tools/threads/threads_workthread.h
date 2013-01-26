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
#ifndef THREADS_WORKTHREAD_H_
#define THREADS_WORKTHREAD_H_

#include "threads_mutex.h"
#include "threads_thread.h"
#include "threads_condition.h"


THREADS_NS_BEGIN

class WorkThread : public Thread
{
public:
	WorkThread();

	volatile bool isStoped() const { return m_stoped; }
	void start(Priority priority);
	void stop();
	void stop(bool wait);
    void terminate();

protected:
	const volatile bool &stopedFlag() const { return m_stoped; }

protected:
	virtual void run();
	virtual void execute() = 0;

private:
	bool m_stoped;
	bool m_abortRequested;
	Mutex m_mutex;
	Condition m_condStoped;
	Condition m_condStopAccepted;
};

THREADS_NS_END

#endif /* THREADS_WORKTHREAD_H_ */
