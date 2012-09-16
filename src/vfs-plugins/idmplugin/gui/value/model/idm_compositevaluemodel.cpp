#include "idm_compositevaluemodel.h"
#include "items/idm_compositevaluevalueitem.h"
#include "items/idm_compositevaluepropertyitem.h"
#include "items/idm_compositevaluerealpathitem.h"
#include "items/idm_compositevaluepossiblediritem.h"
#include "items/idm_compositevaluepossiblefileitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueModel::CompositeValueModel(const IdmEntityValue::Holder &value, QObject *parent) :
	Model(parent)
{
	ValueList list;
	CompositeValuePropertyItem *item;

	for (IdmEntity::size_type i = 0, size = value->entity()->size(); i < size; ++i)
	{
		m_items.push_back(item = new CompositeValuePropertyItem(value->entity()->at(i)));
		list = value.as<IdmCompositeEntityValue>()->values(value->entity()->at(i).entity);

		for (ValueList::size_type i = 0, size = list.size(); i < size; ++i)
			if (list.at(i)->entity()->type() == Database::Path)
				item->add(new CompositeValueRealPathItem(list.at(i), item));
			else
				item->add(new CompositeValueValueItem(list.at(i), item));
	}
}

CompositeValueModel::CompositeValueModel(const IdmEntityValue::Holder &value, const Files &files, QObject *parent) :
	Model(parent)
{
	ValueList list;
	const SnapshotItem *file;
	CompositeValuePropertyItem *item;

	for (IdmEntity::size_type i = 0, size = value->entity()->size(); i < size; ++i)
	{
		m_items.push_back(item = new CompositeValuePropertyItem(value->entity()->at(i)));
		list = value.as<IdmCompositeEntityValue>()->values(value->entity()->at(i).entity);

		for (ValueList::size_type i = 0, size = list.size(); i < size; ++i)
			if (list.at(i)->entity()->type() == Database::Path)
			{
				file = files.value(list.at(i)->id());

				if (file->info()->isFile())
					item->add(new CompositeValuePossibleFileItem(list.at(i), file, item));
				else
					item->add(new CompositeValuePossibleDirItem(list.at(i), file, item));
			}
			else
				item->add(new CompositeValueValueItem(list.at(i), item));
	}
}

void CompositeValueModel::add(const QModelIndex &index, const IdmEntityValue::Holder &value)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());
	item->add(new CompositeValueValueItem(value, item));
	endInsertRows();
}

void CompositeValueModel::add(const QModelIndex &index, const ValueList &values)
{
	CompositeValuePropertyItem *item = static_cast<CompositeValuePropertyItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size() + values.size() - 1);

	for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
		item->add(new CompositeValueValueItem(values.at(i), item));

	endInsertRows();
}

void CompositeValueModel::remove(const QModelIndex &index)
{
	CompositeValueValueItem *item = static_cast<CompositeValueValueItem*>(index.internalPointer());

	beginRemoveRows(parent(index), index.row(), index.row());
	static_cast<CompositeValuePropertyItem*>(item->parent())->remove(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
