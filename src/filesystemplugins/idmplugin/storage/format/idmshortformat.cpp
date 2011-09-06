#include "idmshortformat.h"


IDM_PLUGIN_NS_BEGIN

IdmShortFormat::IdmShortFormat(const QString &format) :
	m_format(format)
{}

bool IdmShortFormat::isValid() const
{
	return true;
}

IDM_PLUGIN_NS_END
