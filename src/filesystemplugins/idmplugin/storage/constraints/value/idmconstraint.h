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
	Constraint(IdmEntity *property, Operator op);

	/* BaseConstraint */
	virtual bool isGroup() const;

	IdmEntity *property() const { return m_property; }
	Operator op() const { return m_op; }

private:
	IdmEntity *m_property;
	Operator m_op;
};

IDM_PLUGIN_NS_END

#endif /* IDMCONSTRAINT_H_ */
