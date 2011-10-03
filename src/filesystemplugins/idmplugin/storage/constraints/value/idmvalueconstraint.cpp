#include "idmvalueconstraint.h"


IDM_PLUGIN_NS_BEGIN

ValueConstraint::ValueConstraint(IdmEntity *property, Operator op, const QVariant &value) :
	Constraint(property, op),
	m_value(value)
{}

QString ValueConstraint::toString() const
{
	return m_value.toString();
}

IDM_PLUGIN_NS_END
