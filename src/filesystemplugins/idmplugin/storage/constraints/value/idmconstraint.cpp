#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

Constraint::Constraint(const IdmEntity::Property &property, Operator op, BaseConstraint *parent) :
	BaseConstraint(parent),
	m_property(property),
	m_op(op)
{}

bool Constraint::isGroup() const
{
	return false;
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
	}
}

IDM_PLUGIN_NS_END
