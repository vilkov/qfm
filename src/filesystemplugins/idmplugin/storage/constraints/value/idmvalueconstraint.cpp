#include "idmvalueconstraint.h"


IDM_PLUGIN_NS_BEGIN

ValueConstraint::ValueConstraint(const IdmEntity::Property &property, Operator op, const QVariant &value, BaseConstraint *parent) :
	Constraint(property, op, parent),
	m_value(value)
{}

QString ValueConstraint::toString() const
{
	return m_value.toString();
}

IDM_PLUGIN_NS_END
