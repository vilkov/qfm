#ifndef IDM_CONSTRAINT_H_
#define IDM_CONSTRAINT_H_

#include "../idm_baseconstraint.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class Constraint : public BaseConstraint
{
public:
	enum Operator
	{
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		Like
	};

public:
	Constraint(const IdmEntity::Property &property, Operator op, const IdmEntityValue::Holder &value, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual bool isGroup() const;
	virtual QString toString() const;

	const IdmEntity::Property &property() const { return m_property; }
	Operator op() const { return m_op; }
	const IdmEntityValue::Holder &value() const { return m_value; }

	static QString operatorToString(Operator op);

private:
	static QString operatorToString(Operator op, Database::EntityType type, const QVariant &value);

private:
	IdmEntity::Property m_property;
	Operator m_op;
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CONSTRAINT_H_ */
