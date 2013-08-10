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
#ifndef VFS_BASETASK_H_
#define VFS_BASETASK_H_

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <tools/taskspool/task.h>
#include "../interfaces/vfs_icopycontrol.h"


VFS_NS_BEGIN
class TasksNode;


class BaseTask : public ::Tools::TasksPool::Task
{
public:
	class Event : public QEvent
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			Question = User,
			UserInput = User + 1,
			Progress = User + 2,
			Completed = User + 3,
			Action = User + 4,

			/* First user event id. */
			User = Action + 1
		};

	public:
		BaseTask *task;
		bool canceled;

	protected:
		Event(BaseTask *task, Type type, bool canceled) :
			QEvent(static_cast<QEvent::Type>(type)),
			task(task),
			canceled(canceled)
		{}
	};

	class ExtendedEvent : public BaseTask::Event
	{
		Q_DISABLE_COPY(ExtendedEvent)

    public:
        typedef BaseTask::Event::Type Type;

    public:
		ICopyControl::Holder destination;

	protected:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled) :
			BaseTask::Event(task, type, canceled),
			destination(destination.take())
		{}
	};

public:
	BaseTask(TasksNode *receiver);
	BaseTask(TasksNode *receiver, ICopyControl::Holder &destination);

    void cancel() { m_canceled = true; }
	const ICopyControl::Holder &destination() const { return m_destination; }

protected:
	ICopyControl::Holder &destination() { return m_destination; }

	/* Posts event to receiver(). */
	void postEvent(Event *event) const;

	/* Sync post event to the GUI thread. */
	qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const;
	qint32 askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile Flags &aborted) const;

private:
	friend class TasksNode;
	void taskHandled();

private:
    TasksNode *m_receiver;
	StaticFlag m_canceled;
    ICopyControl::Holder m_destination;
};

VFS_NS_END

#endif /* VFS_BASETASK_H_ */
