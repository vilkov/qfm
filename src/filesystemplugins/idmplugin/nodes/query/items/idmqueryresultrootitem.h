#ifndef IDMQUERYRESULTROOTITEM_H_
#define IDMQUERYRESULTROOTITEM_H_

#include "../../../model/items/idmlistitem.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootItem : public IdmListItem
{
public:
	QueryResultRootItem(IdmEntityValue *value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

private:
	PScopedPointer<IdmEntityValue> m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTROOTITEM_H_ */
