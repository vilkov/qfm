#ifndef IDMQUERYRESULTROOTITEM_H_
#define IDMQUERYRESULTROOTITEM_H_

#include "idmqueryresultitem.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootItem : public QueryResultItem
{
public:
	QueryResultRootItem(IdmEntityValue *value, IdmItem *parent = 0);

private:
	PScopedPointer<IdmEntityValue> m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTROOTITEM_H_ */
