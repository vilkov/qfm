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
#ifndef PCONDITION_BOOST_H_
#define PCONDITION_BOOST_H_

#include <boost/thread/condition.hpp>
#include "pmutex_boost.h"


class PCondition
{
public:
	PCondition()
	{}

    void wait(PMutex &mutex) { m_condition.wait(mutex); }
    void wakeOne() { m_condition.notify_one(); }
    void wakeAll() { m_condition.notify_all(); }

private:
    boost::condition m_condition;
};

#endif /* PCONDITION_BOOST_H_ */
