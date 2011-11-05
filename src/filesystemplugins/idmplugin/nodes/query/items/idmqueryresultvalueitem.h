#ifndef IDMQUERYRESULTVALUEITEM_H_
#define IDMQUERYRESULTVALUEITEM_H_

#include "../../../model/items/idmitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultValueItem : public IdmItem
{
public:
	QueryResultValueItem(IdmEntityValue *value, IdmItem *parent);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

private:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTVALUEITEM_H_ */
