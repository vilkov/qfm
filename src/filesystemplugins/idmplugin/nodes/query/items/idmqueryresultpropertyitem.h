#ifndef IDMQUERYRESULTPROPERTYITEM_H_
#define IDMQUERYRESULTPROPERTYITEM_H_

#include "idmqueryresultitem.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPropertyItem : public QueryResultItem
{
public:
	typedef QList<QueryResultItem *> Container;

public:
	QueryResultPropertyItem(const IdmEntity::Property &property, Base *parent);
	virtual ~QueryResultPropertyItem();

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

	const IdmEntity::Property &property() const { return m_property; }

	void add(const IFileContainer *container, const IdmEntityValue::Holder &value);
	void add(const IFileContainer *container, const IdmCompositeEntityValue::List &values);
	void add(TasksNode::TasksItemList &files, const IFileContainer *container, const IdmCompositeEntityValue::List &values);
	void remove(size_type index);

private:
	IdmEntity::Property m_property;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPROPERTYITEM_H_ */
