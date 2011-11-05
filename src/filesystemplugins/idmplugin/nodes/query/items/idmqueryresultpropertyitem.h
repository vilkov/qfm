#ifndef IDMQUERYRESULTPROPERTYITEM_H_
#define IDMQUERYRESULTPROPERTYITEM_H_

#include "../../../model/items/idmlistitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPropertyItem : public IdmListItem
{
public:
	QueryResultPropertyItem(const IdmEntity::Property &property, const IdmCompositeEntityValue::List &values, IdmItem *parent);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

private:
	const IdmEntity::Property &m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPROPERTYITEM_H_ */
