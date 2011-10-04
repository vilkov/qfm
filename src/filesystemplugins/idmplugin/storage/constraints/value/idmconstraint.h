#ifndef IDMCONSTRAINT_H_
#define IDMCONSTRAINT_H_

#include "../idmbaseconstraint.h"


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
	Constraint(const IdmEntity::Property &property, Operator op, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual bool isGroup() const;

	const IdmEntity::Property &property() const { return m_property; }
	Operator op() const { return m_op; }

	static QString operatorToString(Operator op);

private:
	const IdmEntity::Property &m_property;
	Operator m_op;
};

IDM_PLUGIN_NS_END

#endif /* IDMCONSTRAINT_H_ */
