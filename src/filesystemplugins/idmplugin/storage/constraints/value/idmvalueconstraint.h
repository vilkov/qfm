#ifndef IDMVALUECONSTRAINT_H_
#define IDMVALUECONSTRAINT_H_

#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

class ValueConstraint : public Constraint
{
public:
	ValueConstraint(IdmEntity *property, Operator op, const QVariant &value);

	/* BaseConstraint */
	virtual QString toString() const;

private:
	QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUECONSTRAINT_H_ */
