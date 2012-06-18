#include "settingswidgetscope.h"


SETTINGS_NS_BEGIN

WidgetScope::WidgetScope(const QString &id, Option *parent) :
	Scope(id, parent),
	m_geometry(QString::fromLatin1("Geometry"), this)
{
	manage(&m_geometry);
}

SETTINGS_NS_END
