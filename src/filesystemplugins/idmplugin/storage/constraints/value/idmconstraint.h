#ifndef IDMCONSTRAINT_H_
#define IDMCONSTRAINT_H_

#include <QtCore/QCoreApplication>
#include "../idmbaseconstraint.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class Constraint : public BaseConstraint
{
	Q_DECLARE_TR_FUNCTIONS(Constraint)

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
	Constraint(const IdmEntity::Property &property, Operator op, IdmEntityValue *value, BaseConstraint *parent = 0);
	virtual ~Constraint();

	/* BaseConstraint */
	virtual bool isGroup() const;
	virtual QString toString() const;

	const IdmEntity::Property &property() const { return m_property; }
	Operator op() const { return m_op; }
	IdmEntityValue *value() const { return m_value; }

	static QString operatorToString(Operator op);

private:
	const IdmEntity::Property &m_property;
	Operator m_op;
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMCONSTRAINT_H_ */
