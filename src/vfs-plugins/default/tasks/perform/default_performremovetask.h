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
#ifndef DEFAULT_PERFORMREMOVETASK_H_
#define DEFAULT_PERFORMREMOVETASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include "../default_filesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformRemoveTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveBaseTask)

public:
	PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);

protected:
	void removeEntry(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void doRemove(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	Snapshot m_snapshot;
	TaskProgress m_progress;

private:
	QString m_error;
	bool m_skipAllIfNotRemove;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PERFORMREMOVETASK_H_ */
