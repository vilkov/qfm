#include "default_openwithaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

OpenWithAction::OpenWithAction(const IApplication *application) :
	SyncFileAction(application->icon(), label(application)),
	m_application(application)
{}

void OpenWithAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::open(m_application, container, files.at(0).second);
}

QString OpenWithAction::label(const IApplication *application)
{
	QString description = application->genericName().isEmpty() ? application->description() : application->genericName();

	if (description.isEmpty())
		return application->name();
	else
		return QString::fromLatin1("%1 (%2)").arg(application->name()).arg(description);
}

DEFAULT_PLUGIN_NS_END
