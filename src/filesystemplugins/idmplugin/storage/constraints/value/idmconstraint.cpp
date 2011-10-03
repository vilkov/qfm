#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

Constraint::Constraint(IdmEntity *property, Operator op)
{}

bool Constraint::isGroup() const
{
	return false;
}

IDM_PLUGIN_NS_END
