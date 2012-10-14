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
#ifndef DEFAULT_PERFORMCOPYTASK_H_
#define DEFAULT_PERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include "../default_filesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformCopyTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

public:
	class ExtendedEvent : public FilesBaseTask::ExtendedEvent
	{
	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			FilesBaseTask::ExtendedEvent(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

	void copyEntry(const IFileContainer *destination, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void copyFile(const IFileContainer *destination, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	QString m_lastError;
	TaskProgress m_progress;

private:
	Snapshot m_snapshot;
	bool m_move;

	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;

	PScopedPointer<IFileAccessor> m_destFile;
	PScopedPointer<IFileAccessor> m_sourceFile;
	IFileAccessor::size_type m_readed;
	IFileAccessor::size_type m_written;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PERFORMCOPYTASK_H_ */
