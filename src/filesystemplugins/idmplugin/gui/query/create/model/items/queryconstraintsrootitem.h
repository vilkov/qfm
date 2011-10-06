#ifndef QUERYCONSTRAINTSROOTITEM_H_
#define QUERYCONSTRAINTSROOTITEM_H_

#include "../../../../../storage/constraints/group/idmgroupconstraint.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsRootItem : public GroupConstraint
{
public:
	QueryConstraintsRootItem();

	void clear();
};

IDM_PLUGIN_NS_END

#endif /* QUERYCONSTRAINTSROOTITEM_H_ */
