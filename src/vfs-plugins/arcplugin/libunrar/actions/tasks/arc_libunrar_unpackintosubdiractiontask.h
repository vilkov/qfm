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
#ifndef ARC_LIBUNRAR_UNPACKINTOSUBDIRACTIONTASK_H_
#define ARC_LIBUNRAR_UNPACKINTOSUBDIRACTIONTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tools/vfs_filestree.h>
#include <vfs/tasks/vfs_performactiontask.h>
#include <tools/taskspool/tryier.h>
#include "../../arc_libunrar_ns.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class UnPackIntoSubdirActionTask : public PerformActionTask
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirActionTask)

public:
	UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files);

protected:
	virtual void process(const volatile Flags &aborted, QString &error);

private:
	typedef Tools::TasksPool::Tryier<UnPackIntoSubdirActionTask> Tryier;

	class CreateDestination
	{
	public:
		CreateDestination(const IFileContainer *container, const IFileInfo *file, IFileContainer::Holder &result) :
			m_container(container),
			m_file(file),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainer *m_container;
		const IFileInfo *m_file;
		IFileContainer::Holder &m_result;
	};

	bool askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Flags &aborted);

private:
	static int callbackProc(unsigned int msg, long userData, long rarBuffer, long bytesProcessed);

private:
	const IFileInfo *m_file;
	const IFileContainer *m_container;
	const volatile Flags *m_aborted;
};

ARC_PLUGIN_LIBUNRAR_NS_END

#endif /* ARC_LIBUNRAR_UNPACKINTOSUBDIRACTIONTASK_H_ */
