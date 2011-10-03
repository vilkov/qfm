#ifndef IDMBASECONSTRAINT_H_
#define IDMBASECONSTRAINT_H_

#include "../entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class BaseConstraint
{
public:
	virtual ~BaseConstraint();

	virtual bool isGroup() const = 0;
	virtual QString toString() const = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMBASECONSTRAINT_H_ */
