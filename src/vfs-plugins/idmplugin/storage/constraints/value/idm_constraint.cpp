#include "idm_constraint.h"


IDM_PLUGIN_NS_BEGIN

Constraint::Constraint(const Entity::Property &property, Operator op, const EntityValue::Holder &value, BaseConstraint *parent) :
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
	if (m_value->id() == EntityValue::InvalidId)
		return QString::fromLatin1("ENTITY_%1.VALUE").
				arg(QString::number(m_property.entity->id())).
				append(operatorToString(m_op, m_property.entity->type(), m_value->value()));
	else
		return QString::fromLatin1("ENTITY_%1.ID = ").
				arg(QString::number(m_property.entity->id())).
				append(QString::number(m_value->id()));
}

QString Constraint::operatorToString(Operator op)
{
	switch (op)
	{
		case Less:
			return QString::fromLatin1(" < ");

		case LessEqual:
			return QString::fromLatin1(" <= ");

		case Greater:
			return QString::fromLatin1(" > ");

		case GreaterEqual:
			return QString::fromLatin1(" >= ");

		case Equal:
			return QString::fromLatin1(" = ");

		case Like:
			return QString::fromLatin1(" like ");

		default:
			return QString();
	}
}

QString Constraint::operatorToString(Operator op, Database::EntityType type, const QVariant &value)
{
	switch (op)
	{
		case Less:
			return QString::fromLatin1(" < ").append(Database::valueToConstraintString(type, value));

		case LessEqual:
			return QString::fromLatin1(" <= ").append(Database::valueToConstraintString(type, value));

		case Greater:
			return QString::fromLatin1(" > ").append(Database::valueToConstraintString(type, value));

		case GreaterEqual:
			return QString::fromLatin1(" >= ").append(Database::valueToConstraintString(type, value));

		case Equal:
			return QString::fromLatin1(" = ").append(Database::valueToConstraintString(type, value));

		case Like:
			return QString::fromLatin1(" like ").append(Database::valueToConstraintString(type, value).replace(QChar(L'*'), QChar(L'%')));

		default:
			return QString();
	}
}

IDM_PLUGIN_NS_END
