#include "idmidconstraint.h"


IDM_PLUGIN_NS_BEGIN

IdConstraint::IdConstraint(IdmEntity *property, Operator op, IdmEntity::id_type id) :
	Constraint(property, op),
	m_id(id)
{}

QString IdConstraint::toString() const
{
	return QString::number(m_id);
}

IDM_PLUGIN_NS_END
