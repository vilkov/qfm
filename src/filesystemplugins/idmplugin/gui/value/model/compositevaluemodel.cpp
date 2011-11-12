#include "compositevaluemodel.h"
#include "items/compositevaluevalueitem.h"
#include "items/compositevaluepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueModel::CompositeValueModel(IdmEntity *entity, QObject *parent) :
	IdmModel(parent),
	m_valueOwner(true)
{
	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
		m_items.push_back(new CompositeValuePropertyItem(entity->at(i)));
}

CompositeValueModel::CompositeValueModel(IdmCompositeEntityValue *value, QObject *parent) :
	IdmModel(parent),
	m_valueOwner(false)
{
	ValueList list;
	CompositeValuePropertyItem *item;

	for (IdmEntity::size_type i = 0, size = value->entity()->size(); i < size; ++i)
	{
		m_items.push_back(item = new CompositeValuePropertyItem(value->entity()->at(i)));
		list = value->values(value->entity()->at(i).entity);

		if (!list.isEmpty())
			for (ValueList::size_type i = 0, size = list.size(); i < size; ++i)
				item->add(new CompositeValueValueItem(list.at(i), item));
	}
}

void CompositeValueModel::add(const QModelIndex &index, IdmEntityValue *value)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());

	if (m_valueOwner)
		item->add(new CompositeValueValueOwnerItem(value, item));
	else
		item->add(new CompositeValueValueItem(value, item));

	endInsertRows();
}

void CompositeValueModel::add(const QModelIndex &index, const ValueList &values)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size() + values.size() - 1);

	if (m_valueOwner)
		for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
			item->add(new CompositeValueValueOwnerItem(values.at(i), item));
	else
		for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
			item->add(new CompositeValueValueItem(values.at(i), item));

	endInsertRows();
}

void CompositeValueModel::remove(const QModelIndex &index)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginRemoveRows(parent(index), index.row(), index.row());

	if (item->parent())
		static_cast<CompositeValuePropertyItem*>(item->parent())->remove(index.row());
	else
		delete m_items.takeAt(index.row());

	endRemoveRows();
}

IDM_PLUGIN_NS_END
