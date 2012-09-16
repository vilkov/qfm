#ifndef IDM_QUERYCONSTRAINTSROOTITEM_H_
#define IDM_QUERYCONSTRAINTSROOTITEM_H_

#include "../../../../../storage/constraints/group/idm_groupconstraint.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsRootItem : public GroupConstraint
{
public:
	QueryConstraintsRootItem();

	void clear();
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYCONSTRAINTSROOTITEM_H_ */
