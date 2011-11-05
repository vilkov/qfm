#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include "../../../model/items/idmlistitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultItem : public IdmListItem
{
public:
	QueryResultItem(IdmEntityValue *value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

private:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */
