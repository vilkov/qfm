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
#ifndef PTHREAD_QT_H_
#define PTHREAD_QT_H_

#include <QtCore/QThread>
#include "../threadbase.h"


class PThread : public ThreadBase
{
public:
	PThread() :
		m_holder(this)
	{}

	virtual void start() { m_holder.start(Holder::InheritPriority); }
	virtual void terminate() { m_holder.terminate(); }

	void wait() { m_holder.wait(); }

private:
    class Holder : public QThread
    {
    public:
    	Holder(PThread *thread) :
    		QThread(),
        	m_thread(thread)
    	{}

    protected:
        virtual void run() { m_thread->run(); }

    private:
    	PThread *m_thread;
    };
    friend class Holder;

private:
    Holder m_holder;
};

#endif /* PTHREAD_QT_H_ */
