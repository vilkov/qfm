#include "compositevaluemodel.h"
#include "items/compositevaluevalueitem.h"
#include "items/compositevaluepropertyitem.h"
#include "../../../storage/values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueModel::CompositeValueModel(IdmCompositeEntityValue *value, QObject *parent) :
	IdmModel(parent),
	m_value(value)
{
	ValueList list;
	CompositeValuePropertyItem *item;

	for (IdmEntity::size_type i = 0, size = m_value->entity()->size(); i < size; ++i)
	{
		m_items.push_back(item = new CompositeValuePropertyItem(m_value->entity()->at(i)));
		list = m_value->values(m_value->entity()->at(i).entity);

		if (!list.isEmpty())
			for (ValueList::size_type i = 0, size = list.size(); i < size; ++i)
				item->add(new CompositeValueValueItem(list.at(i), item));
	}
}

void CompositeValueModel::add(const QModelIndex &index, IdmEntityValue *value)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());

	if (m_value == 0)
		item->add(new CompositeValueValueOwnerItem(value, item));
	else
	{
		IdmValueReader::addValue(m_value, value);
		item->add(new CompositeValueValueItem(value, item));
	}

	endInsertRows();
}

void CompositeValueModel::add(const QModelIndex &index, const ValueList &values)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size() + values.size() - 1);

	if (m_value == 0)
		for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
			item->add(new CompositeValueValueOwnerItem(values.at(i), item));
	else
		for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
		{
			IdmValueReader::addValue(m_value, values.at(i));
			item->add(new CompositeValueValueItem(values.at(i), item));
		}

	endInsertRows();
}

void CompositeValueModel::remove(const QModelIndex &index)
{
	CompositeValueValueItem *item = static_cast<CompositeValueValueItem*>(index.internalPointer());

	beginRemoveRows(parent(index), index.row(), index.row());

	if (m_value == 0)
		static_cast<CompositeValuePropertyItem*>(item->parent())->remove(index.row());
	else
	{
		IdmValueReader::removeValue(m_value, item->value());
		static_cast<CompositeValuePropertyItem*>(item->parent())->remove(index.row());
	}

	endRemoveRows();
}

IDM_PLUGIN_NS_END
