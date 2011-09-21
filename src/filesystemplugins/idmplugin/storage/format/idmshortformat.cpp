#include "idmshortformat.h"


IDM_PLUGIN_NS_BEGIN

IdmShortFormat::IdmShortFormat(const QString &format) :
	m_format(format)
{}

bool IdmShortFormat::isValid() const
{
	return !m_format.isEmpty();
}

IDM_PLUGIN_NS_END
