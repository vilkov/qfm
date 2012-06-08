#ifndef LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_
#define LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_

#include "../../libunrar_ns.h"
#include "../../../actions/tasks/arcunpackactiontask.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirActionTask : public UnPackActionTask
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirActionTask)

public:
	UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files);

protected:
	virtual void process(const volatile Flags &aborted, QString &error);

protected:
	virtual IFileContainer *openArchive(const IFileContainer *container, const IFileInfo *file, QString &error) const;
	virtual QString extractArchivedFileName(const IFileInfo *file) const;
	virtual QString extractDirectoryName(const IFileInfo *file) const;
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARUNPACKINTOSUBDIRACTIONTASK_H_ */
