#ifndef IDMIDCONSTRAINT_H_
#define IDMIDCONSTRAINT_H_

#include "idmvalueconstraint.h"


IDM_PLUGIN_NS_BEGIN

class IdConstraint : public ValueConstraint
{
public:
	IdConstraint(const IdmEntity::Property &property, Operator op, const QVariant &value, IdmEntity::id_type id, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual QString toString() const;

	IdmEntity::id_type id() const { return m_id; }

private:
	IdmEntity::id_type m_id;
};

IDM_PLUGIN_NS_END

#endif /* IDMIDCONSTRAINT_H_ */
