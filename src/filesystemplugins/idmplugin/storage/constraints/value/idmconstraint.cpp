#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

Constraint::Constraint(const IdmEntity::Property &property, Operator op, const IdmEntityValue::Holder &value, BaseConstraint *parent) :
	BaseConstraint(parent),
	m_property(property),
	m_op(op),
	m_value(value)
{}

bool Constraint::isGroup() const
{
	return false;
}

QString Constraint::toString() const
{
	return QString();
}

QString Constraint::operatorToString(Operator op)
{
	switch (op)
	{
		case Less:
			return QString::fromLatin1("<");

		case LessEqual:
			return QString::fromLatin1("<=");

		case Greater:
			return QString::fromLatin1(">");

		case GreaterEqual:
			return QString::fromLatin1(">=");

		case Equal:
			return QString::fromLatin1("=");

		case Like:
			return QString::fromLatin1("like");

		default:
			return QString();
	}
}

IDM_PLUGIN_NS_END
