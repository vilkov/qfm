#ifndef IDMVALUECONSTRAINT_H_
#define IDMVALUECONSTRAINT_H_

#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

class ValueConstraint : public Constraint
{
public:
	ValueConstraint(const IdmEntity::Property &property, Operator op, const QVariant &value, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual QString toString() const;

	const QVariant &value() const { return m_value; }

private:
	QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUECONSTRAINT_H_ */
