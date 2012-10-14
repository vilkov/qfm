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
#include "default_searchtask.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchTask::SearchTask(const Snapshot &snapshot, IFileContainerScanner::Filter::Holder &filter, TasksNode *receiver) :
	BaseTask(receiver),
	m_snapshot(snapshot),
	m_filter(filter.take())
{}

void SearchTask::run(const volatile Flags &aborted)
{
	QString error;
	IFileContainerScanner::SearchArguments args = {m_snapshot, m_filter, {newElement, this}, aborted};
	m_snapshot.container()->scanner()->search(args, error);
	postEvent(new DoneEvent(this, aborted));
}

void SearchTask::newElement(void *user_data, SnapshotItem *item)
{
	SearchTask *self(static_cast<SearchTask *>(user_data));
	self->postEvent(new NewFileEvent(self, item));
}

DEFAULT_PLUGIN_NS_END
