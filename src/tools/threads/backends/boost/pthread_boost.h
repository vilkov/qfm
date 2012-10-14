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
#ifndef PTHREAD_BOOST_H_
#define PTHREAD_BOOST_H_

#include <boost/thread/thread.hpp>
#include "../threadbase.h"


class PThread : public ThreadBase
{
public:
	PThread() :
		m_thread(boost::bind(&PThread::run, this))
	{}

	void wait() { m_thread.join(); }

private:
	boost::thread m_thread;
};

#endif /* PTHREAD_BOOST_H_ */
