#ifndef IDMQUERYRESULTROOTITEM_H_
#define IDMQUERYRESULTROOTITEM_H_

#include "idmqueryresultitem.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootItem : public QueryResultListItem
{
public:
	QueryResultRootItem(IdmEntityValue *value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();

	IdmEntityValue *value() const { return m_value.data(); }

private:
	PScopedPointer<IdmEntityValue> m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTROOTITEM_H_ */
