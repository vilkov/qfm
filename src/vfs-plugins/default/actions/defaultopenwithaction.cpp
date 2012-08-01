#include "defaultopenwithaction.h"


DEFAULT_PLUGIN_NS_BEGIN

OpenWithAction::OpenWithAction(const IApplication *application) :
	SyncFileAction(application->icon(), QString::fromLatin1("%1 (%2)").arg(application->name()).arg(application->description()))
{}

void OpenWithAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
