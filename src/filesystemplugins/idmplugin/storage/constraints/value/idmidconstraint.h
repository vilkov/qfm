#ifndef IDMIDCONSTRAINT_H_
#define IDMIDCONSTRAINT_H_

#include "idmconstraint.h"


IDM_PLUGIN_NS_BEGIN

class IdConstraint : public Constraint
{
public:
	IdConstraint(IdmEntity *property, Operator op, IdmEntity::id_type id);

	/* BaseConstraint */
	virtual QString toString() const;

private:
	IdmEntity::id_type m_id;
};

IDM_PLUGIN_NS_END

#endif /* IDMIDCONSTRAINT_H_ */
