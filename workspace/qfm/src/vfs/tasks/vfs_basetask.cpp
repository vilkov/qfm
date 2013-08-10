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
#include "vfs_basetask.h"
#include "vfs_tasksnode.h"
#include "tools/vfs_taskdialog.h"
#include "../../application.h"

#include <QtCore/QThread>


VFS_NS_BEGIN

BaseTask::BaseTask(TasksNode *receiver) :
	Task(),
	m_receiver(receiver),
	m_canceled(1, flags())
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

BaseTask::BaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	Task(),
	m_receiver(receiver),
	m_canceled(1, flags()),
	m_destination(destination.take())
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_destination.data());
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

void BaseTask::postEvent(Event *event) const
{
	Application::postEvent(m_receiver, event);
}

qint32 BaseTask::askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const
{
	QuestionEvent::Result result;
	postEvent(new QuestionEvent(title, question, buttons, &result));
	result.waitFor(aborted);
	return result.answer();
}

qint32 BaseTask::askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile Flags &aborted) const
{
	UserInputEvent::Result result;
	postEvent(new UserInputEvent(title, question, buttons, &result));
	result.waitFor(aborted);
	value = result.value();
	return result.answer();
}

VFS_NS_END
