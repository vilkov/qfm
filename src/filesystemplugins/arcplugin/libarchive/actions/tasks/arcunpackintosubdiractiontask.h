#ifndef ARCUNPACKINTOSUBDIRACTIONTASK_H_
#define ARCUNPACKINTOSUBDIRACTIONTASK_H_

#include <QtCore/QCoreApplication>
#include "../../libarchive_ns.h"
#include "../../../../../filesystem/tasks/filesystemperformactiontask.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirActionTask : public PerformActionTask
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirActionTask)

public:
	UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files);

	virtual void progressInit(const NodeItem::Holder &item);
	virtual void progressUpdate(quint64 progressIncrement);
	virtual void progresscomplete();

	virtual bool overwriteAll() const;
	virtual bool skipAllIfNotCopy() const;
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	virtual void process(const volatile Flags &aborted);

private:
	QString folderName(const QString &fileName) const;

private:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

private:
	bool m_overwriteAll;
	bool m_skipAllIfNotCopy;
	TaskProgress m_progress;
	const IFileContainer *m_container;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* ARCUNPACKINTOSUBDIRACTIONTASK_H_ */
