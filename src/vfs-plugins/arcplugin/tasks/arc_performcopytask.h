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
#ifndef ARC_PERFORMCOPYTASK_H_
#define ARC_PERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include <vfs/interfaces/vfs_icopycontrol.h>
#include "arc_filesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class PerformCopyTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
//	class Event : public FilesBaseTask::Event
//	{
//	public:
//		Event(BaseTask *task, bool canceled, bool move) :
//			FilesBaseTask::Event(task, CopyComplete, canceled),
//			move(move)
//		{}
//
//		ArcNodeItem::Holder item;
//		bool move;
//	};

public:
	PerformCopyTask(const QString &fileName, const Item::Holder &item, ICopyControl::Holder &control, bool move, TasksNode *receiver);

	virtual IFileAccessor::value_type *buffer() const;
	virtual IFileAccessor::size_type bufferSize() const;

	virtual void progressInit(const Item::Holder &item);
	virtual void progressUpdate(quint64 progressIncrement);
	virtual void progresscomplete();

	virtual bool overwriteAll() const;
	virtual bool skipAllIfNotCopy() const;
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

private:
	QString m_fileName;
	Item::Holder m_item;
	ICopyControl::Holder m_control;
	bool m_move;
	bool m_overwriteAll;
	bool m_skipAllIfNotCopy;
	TaskProgress m_progress;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

ARC_PLUGIN_NS_END

#endif /* ARC_PERFORMCOPYTASK_H_ */
