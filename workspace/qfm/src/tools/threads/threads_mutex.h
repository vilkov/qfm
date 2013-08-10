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
#ifndef THREADS_MUTEX_H_
#define THREADS_MUTEX_H_

#include <QtCore/QMutex>
#include "threads_ns.h"


THREADS_NS_BEGIN

class Mutex
{
public:
    class Locker
    {
    public:
        explicit Locker(Mutex &mutex) :
            m_locker(&mutex.m_mutex)
        {}

        void lock() { m_locker.relock(); }
        void unlock() { m_locker.unlock(); }

    private:
        QMutexLocker m_locker;
    };

    class Unlocker
    {
    public:
        explicit Unlocker(Locker &locker) :
            m_locker(locker)
        {
            m_locker.unlock();
        }
        ~Unlocker()
        {
            m_locker.lock();
        }

    private:
        Locker &m_locker;
    };

public:
    Mutex()
    {}

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

private:
    friend class Condition;
    QMutex m_mutex;
};

THREADS_NS_END

#endif /* THREADS_MUTEX_H_ */
