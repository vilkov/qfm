#include "idmgroupconstraint.h"


IDM_PLUGIN_NS_BEGIN

GroupConstraint::GroupConstraint(Type type) :
	m_type(type)
{}

bool GroupConstraint::isGroup() const
{
	return true;
}

QString GroupConstraint::toString() const
{
	return QString();
}

IDM_PLUGIN_NS_END
