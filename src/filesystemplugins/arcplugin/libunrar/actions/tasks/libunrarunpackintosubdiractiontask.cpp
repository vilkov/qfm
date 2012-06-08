#include "libunrarunpackintosubdiractiontask.h"
#include "../../interfaces/libunrarfilecontainer.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirActionTask::UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files) :
	UnPackActionTask(receiver, container, files)
{}

void UnPackIntoSubdirActionTask::process(const volatile Flags &aborted, QString &error)
{
	unpackIntoSubfolder(aborted, error);
}

IFileContainer *UnPackIntoSubdirActionTask::openArchive(const IFileContainer *container, const IFileInfo *file, QString &error) const
{
	return FileContainer::create(container, file, error);
}

QString UnPackIntoSubdirActionTask::extractArchivedFileName(const IFileInfo *file) const
{
	return FileContainer::extractArchivedFileName(file);
}

QString UnPackIntoSubdirActionTask::extractDirectoryName(const IFileInfo *file) const
{
	return FileContainer::extractDirectoryName(file);
}

LIBUNRAR_ARC_PLUGIN_NS_END
