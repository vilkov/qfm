#include "defaultopenwithaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

OpenWithAction::OpenWithAction(const IApplication *application) :
	SyncFileAction(application->icon(),
			application->description().isEmpty() ?
					application->name() :
					QString::fromLatin1("%1 (%2)").
						arg(application->name()).
						arg(application->description())),
	m_application(application)
{}

void OpenWithAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::open(m_application, container, files.at(0).second);
}

DEFAULT_PLUGIN_NS_END
