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
#include "vfs_taskprogress.h"
#include "vfs_taskprogressevents.h"
#include "../vfs_tasksnode.h"
#include "../../../application.h"


VFS_NS_BEGIN

TaskProgress::TaskProgress(TasksNode *receiver) :
	m_item(NULL),
	m_receiver(receiver)
{}


void TaskProgress::init(const Item::Holder &item)
{
	Q_ASSERT(item);
	m_item = item;
	m_progress = 0;
	m_baseTime = m_currentTime = m_startTime = QDateTime::currentDateTime();
}

void TaskProgress::update(quint64 progressIncrement)
{
	Q_ASSERT(m_item);
	m_progress += progressIncrement;

	if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
	{
		postEvent();
		m_baseTime = m_currentTime;
	}
}

void TaskProgress::complete()
{
	typedef CompletedProgressEvent Event;

	::Tools::Memory::ScopedPointer<Event> event(new Event(m_item, m_startTime.msecsTo(QDateTime::currentDateTime())));
	Application::postEvent(m_receiver, event.take());
}

void TaskProgress::clear()
{
	m_item = NULL;
}

void TaskProgress::postEvent()
{
	typedef UpdateProgressEvent Event;

	::Tools::Memory::ScopedPointer<Event> event(new Event(m_item, m_progress, m_startTime.msecsTo(m_currentTime)));
	Application::postEvent(m_receiver, event.take());
}

VFS_NS_END
