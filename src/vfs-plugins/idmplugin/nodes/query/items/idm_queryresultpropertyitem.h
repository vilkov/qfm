#ifndef IDM_QUERYRESULTPROPERTYITEM_H_
#define IDM_QUERYRESULTPROPERTYITEM_H_

#include "idm_queryresultitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPropertyItem : public QueryResultItem
{
public:
	typedef QList<QueryResultItem::Holder> Container;

public:
	QueryResultPropertyItem(const Entity::Property &property, Base *parent);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;
	virtual QVariant data(qint32 column, qint32 role) const;

	/* QueryResultItem */
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();
	virtual bool isPath();
	virtual bool isRootPathValue();

	const Entity::Property &property() const { return m_property; }

	void add(const EntityValue::Holder &value);
	void add(const CompositeEntityValue::List &values);
	void remove(size_type index);

protected:
	Entity::Property m_property;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTPROPERTYITEM_H_ */
