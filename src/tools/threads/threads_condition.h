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
#ifndef THREADS_CONDITION_H_
#define THREADS_CONDITION_H_

#include <QtCore/QWaitCondition>
#include "threads_mutex.h"


THREADS_NS_BEGIN

class Condition
{
public:
    Condition()
    {}

    void wait(Mutex &mutex) { m_condition.wait(&mutex.m_mutex); }
    bool wait(Mutex &mutex, unsigned long time) { return m_condition.wait(&mutex.m_mutex, time); }
    void wakeOne() { m_condition.wakeOne(); }
    void wakeAll() { m_condition.wakeAll(); }

private:
    QWaitCondition m_condition;
};

THREADS_NS_END

#endif /* THREADS_CONDITION_H_ */
