#include "idmidconstraint.h"


IDM_PLUGIN_NS_BEGIN

IdConstraint::IdConstraint(const IdmEntity::Property &property, Operator op, const QVariant &value, IdmEntity::id_type id, BaseConstraint *parent) :
	ValueConstraint(property, op, value, parent),
	m_id(id)
{}

QString IdConstraint::toString() const
{
	return QString::number(m_id);
}

IDM_PLUGIN_NS_END
