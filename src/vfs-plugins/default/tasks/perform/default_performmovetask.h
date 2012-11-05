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
#ifndef PERFORMMOVETASK_H_
#define PERFORMMOVETASK_H_

#include "default_performcopytask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformMoveTask : public PerformCopyTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformMoveTask)

public:
	PerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot);

protected:
    virtual void copyFile(const IFileContainer *destination, SnapshotItem *entry, const volatile Flags &aborted);

private:
    QString m_lastError;
};

DEFAULT_PLUGIN_NS_END

#endif /* PERFORMMOVETASK_H_ */
