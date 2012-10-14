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
#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>


class WorkThread : public QThread
{
	Q_OBJECT

public:
	WorkThread(QObject *parent = 0);

	volatile bool isStoped() const { return m_stoped; }
	void start(const Priority &priority = NormalPriority);
	void stop();
	void stop(bool wait);
    void terminate();

Q_SIGNALS:
	void done();
	void abortRequested();

protected:
	const volatile bool &stopedFlag() const { return m_stoped; }

protected:
	virtual void run();
	virtual void execute() = 0;

private:
	QMutex m_mutex;
	bool m_stoped;
	bool m_abortRequested;
	QWaitCondition m_condStoped;
	QWaitCondition m_condStopAccepted;
};

#endif /* WORKTHREAD_H_ */
