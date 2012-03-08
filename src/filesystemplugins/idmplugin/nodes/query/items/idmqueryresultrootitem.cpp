#include "idmqueryresultrootitem.h"
#include "idmqueryresultpropertyitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultRootItem::QueryResultRootItem(TasksNode::TasksItemList &files, const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultListItem(parent),
	m_value(value)
{
	if (m_value->entity()->type() == Database::Composite)
	{
		PScopedPointer<QueryResultPropertyItem> item;

		for (IdmEntity::size_type i = 0, size = m_value->entity()->size(); i < size; ++i)
		{
			const IdmEntity::Property &poperty = m_value->entity()->at(i);
			item = new QueryResultPropertyItem(poperty, this);
			item->add(files, container, static_cast<IdmCompositeEntityValue*>(m_value.data())->values(poperty.entity));
			m_items.push_back(item.take());
		}
	}
}

QVariant QueryResultRootItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool QueryResultRootItem::isRoot()
{
	return true;
}

bool QueryResultRootItem::isProperty()
{
	return false;
}

bool QueryResultRootItem::isValue()
{
	return false;
}

IDM_PLUGIN_NS_END
