#include "idmqueryresultcompositerootitem.h"
#include "idmqueryresultpropertyitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultCompositeRootItem::QueryResultCompositeRootItem(TasksNode::TasksItemList &files, const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultRootItem(value, parent),
	m_items(value->entity()->size())
{
	QueryResultPropertyItem *item;

	for (IdmEntity::size_type i = 0, size = value->entity()->size(); i < size; ++i)
	{
		const IdmEntity::Property &poperty = value->entity()->at(i);
		m_items[i] = item = new QueryResultPropertyItem(poperty, this);
		item->add(files, container, value.as<IdmCompositeEntityValue>()->values(poperty.entity));
	}
}

QueryResultCompositeRootItem::~QueryResultCompositeRootItem()
{
	qDeleteAll(m_items);
}

QueryResultCompositeRootItem::Base *QueryResultCompositeRootItem::at(size_type index) const
{
	return m_items.at(index);
}

QueryResultCompositeRootItem::size_type QueryResultCompositeRootItem::size() const
{
	return m_items.size();
}

QueryResultCompositeRootItem::size_type QueryResultCompositeRootItem::indexOf(Base *item) const
{
	return m_items.indexOf(static_cast<QueryResultItem *>(item));
}

IDM_PLUGIN_NS_END
