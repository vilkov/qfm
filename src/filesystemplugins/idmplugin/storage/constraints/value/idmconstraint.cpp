#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

Constraint::Constraint(const IdmEntity::Property &property, Operator op, IdmEntityValue *value, BaseConstraint *parent) :
	BaseConstraint(parent),
	m_property(property),
	m_op(op),
	m_value(value)
{}

Constraint::~Constraint()
{
	delete m_value;
}

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
			return tr("<");

		case LessEqual:
			return tr("<=");

		case Greater:
			return tr(">");

		case GreaterEqual:
			return tr(">=");

		case Equal:
			return tr("=");

		case Like:
			return tr("like");
	}
}

IDM_PLUGIN_NS_END
