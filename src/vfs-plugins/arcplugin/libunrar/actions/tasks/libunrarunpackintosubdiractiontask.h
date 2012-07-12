#ifndef LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_
#define LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tools/vfs_filestree.h>
#include <vfs/tasks/vfs_performactiontask.h>
#include "../../libunrar_ns.h"
#include "../../../../../tools/taskspool/tryier.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

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

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_ */
