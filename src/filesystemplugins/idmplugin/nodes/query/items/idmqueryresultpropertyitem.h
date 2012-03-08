#ifndef IDMQUERYRESULTPROPERTYITEM_H_
#define IDMQUERYRESULTPROPERTYITEM_H_

#include "idmqueryresultitem.h"
#include "idmqueryresultrootitem.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPropertyItem : public QueryResultListItem
{
public:
	QueryResultPropertyItem(const IFileContainer *container, const IdmEntity::Property &property, const IdmCompositeEntityValue::List &values, Base *parent);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();

	const IdmEntity::Property &property() const { return m_property; }
	const IdmEntityValue::Holder &rootValue() const { return static_cast<QueryResultRootItem*>(parent())->value(); }

	void add(const IFileContainer *container, const IdmEntityValue::Holder &value);
	void add(const IFileContainer *container, const IdmCompositeEntityValue::List &values);
	void remove(size_type index);

private:
	const IdmEntity::Property &m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPROPERTYITEM_H_ */
