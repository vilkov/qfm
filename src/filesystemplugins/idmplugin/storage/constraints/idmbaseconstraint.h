#ifndef IDMBASECONSTRAINT_H_
#define IDMBASECONSTRAINT_H_

#include "../entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class BaseConstraint
{
public:
	BaseConstraint(BaseConstraint *parent = 0);
	virtual ~BaseConstraint();

	BaseConstraint *parent() const { return m_parent; }

	virtual bool isGroup() const = 0;
	virtual QString toString() const = 0;

private:
	BaseConstraint *m_parent;
};

IDM_PLUGIN_NS_END

#endif /* IDMBASECONSTRAINT_H_ */
