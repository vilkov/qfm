/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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
#ifndef THREADS_THREAD_H_
#define THREADS_THREAD_H_

#include <QtCore/QThread>
#include "threads_ns.h"


THREADS_NS_BEGIN

class Thread
{
public:
    enum Priority {
        IdlePriority,

        LowestPriority,
        LowPriority,
        NormalPriority,
        HighPriority,
        HighestPriority,

        TimeCriticalPriority,

        InheritPriority
    };

public:
    Thread() :
        m_holder(this)
    {}
    virtual ~Thread();

    bool isRunning() const { return m_holder.isRunning(); }
    void start(Priority priority = InheritPriority) { m_holder.start(static_cast<QThread::Priority>(priority)); }
    void terminate() { m_holder.terminate(); }

    void wait() { m_holder.wait(); }

    Priority priority() const { return static_cast<Priority>(m_holder.priority()); }
    void setPriority(Priority priority) { m_holder.setPriority(static_cast<QThread::Priority>(priority)); }

protected:
    virtual void run() = 0;

private:
    class Holder : public QThread
    {
    public:
        Holder(Thread *thread) :
            QThread(),
            m_thread(thread)
        {}

    protected:
        virtual void run() { m_thread->run(); }

    private:
        Thread *m_thread;
    };
    friend class Holder;

private:
    Holder m_holder;
};

THREADS_NS_END

#endif /* THREADS_THREAD_H_ */
