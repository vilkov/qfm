#include "compositevaluemodel.h"
#include "items/valuestreeitem.h"
#include "items/valuestreevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueModel::CompositeValueModel(IdmEntity *entity, QObject *parent) :
	IdmModel(parent)
{
	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
		m_items.push_back(new ValuesTreeItem(entity->at(i)));
}

void CompositeValueModel::add(const QModelIndex &index, IdmEntityValue *value)
{
	ValuesTreeItem *item = static_cast<ValuesTreeItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());
	item->add(new ValuesTreeValueItem(value, item));
	endInsertRows();
}

void CompositeValueModel::add(const QModelIndex &index, const ValueList &values)
{
	ValuesTreeItem *item = static_cast<ValuesTreeItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size() + values.size() - 1);

	for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
		item->add(new ValuesTreeValueItem(values.at(i), item));

	endInsertRows();
}

void CompositeValueModel::remove(const QModelIndex &index)
{
	ValuesTreeItem *item = static_cast<ValuesTreeItem*>(index.internalPointer());

	beginRemoveRows(parent(index), index.row(), index.row());

	if (item->parent())
		static_cast<ValuesTreeItem*>(item->parent())->remove(index.row());
	else
		delete m_items.takeAt(index.row());

	endRemoveRows();
}

IDM_PLUGIN_NS_END
