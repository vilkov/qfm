#ifndef IDMQUERYRESULTPROPERTYITEM_H_
#define IDMQUERYRESULTPROPERTYITEM_H_

#include "idmqueryresultitem.h"
#include "idmqueryresultrootitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPropertyItem : public QueryResultListItem
{
public:
	QueryResultPropertyItem(const IdmEntity::Property &property, const IdmCompositeEntityValue::List &values, Base *parent);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();

	const IdmEntity::Property &property() const { return m_property; }
	IdmCompositeEntityValue *rootValue() const { return static_cast<IdmCompositeEntityValue*>(static_cast<QueryResultRootItem*>(parent())->value()); }

	void add(IdmEntityValue *value);
	void add(const IdmCompositeEntityValue::List &values);
	void remove(size_type index);

private:
	const IdmEntity::Property &m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPROPERTYITEM_H_ */
