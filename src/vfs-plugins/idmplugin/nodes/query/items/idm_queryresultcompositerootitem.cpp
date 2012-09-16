#include "idm_queryresultcompositerootitem.h"
#include "idm_queryresultpropertyitem.h"
#include "idm_queryresultpathpropertyitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultCompositeRootItem::QueryResultCompositeRootItem(Snapshot::Files &files, const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultRootItem(value, parent),
	m_items(value->entity()->size())
{
	QueryResultPropertyItem::Holder item;

	for (IdmEntity::size_type i = 0, size = value->entity()->size(); i < size; ++i)
	{
		const IdmEntity::Property &poperty = value->entity()->at(i);

		if (poperty.entity->type() == Database::Path)
		{
			item = new QueryResultPathPropertyItem(poperty, this);
			item.as<QueryResultPathPropertyItem>()->add(files, container, value.as<IdmCompositeEntityValue>()->values(poperty.entity));
		}
		else
		{
			item = new QueryResultPropertyItem(poperty, this);
			item.as<QueryResultPropertyItem>()->add(value.as<IdmCompositeEntityValue>()->values(poperty.entity));
		}

		m_items[i] = item;
	}
}

QueryResultCompositeRootItem::Base *QueryResultCompositeRootItem::at(size_type index) const
{
	return m_items.at(index).data();
}

QueryResultCompositeRootItem::size_type QueryResultCompositeRootItem::size() const
{
	return m_items.size();
}

QueryResultCompositeRootItem::size_type QueryResultCompositeRootItem::indexOf(Base *item) const
{
	Holder holder(static_cast<QueryResultItem *>(item));
	return m_items.indexOf(holder);
}

IDM_PLUGIN_NS_END
