/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "idm_compositevaluemodel.h"
#include "items/idm_compositevaluevalueitem.h"
#include "items/idm_compositevaluepropertyitem.h"
#include "items/idm_compositevaluerealpathitem.h"
#include "items/idm_compositevaluepossiblediritem.h"
#include "items/idm_compositevaluepossiblefileitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueModel::CompositeValueModel(const EntityValue &value, QObject *parent) :
    Model(parent)
{
    ValueList list;
    CompositeValuePropertyItem *item;

    for (auto i : value.entity().properties())
    {
        m_items.push_back(item = new CompositeValuePropertyItem(i.second));
        list = CompositeEntityValue(value).values(i.second.entity);

        for (auto i : list)
//            if (i.second.entity().type() == Entity::Path)
//                item->add(new CompositeValueRealPathItem(i.second, item));
//            else
                item->add(new CompositeValueValueItem(i.second, item));
    }
}

CompositeValueModel::CompositeValueModel(const EntityValue &value, const Files &files, QObject *parent) :
    Model(parent)
{
    ValueList list;
    const SnapshotItem *file;
    CompositeValuePropertyItem *item;

    for (auto i : value.entity().properties())
    {
        m_items.push_back(item = new CompositeValuePropertyItem(i.second));
        list = CompositeEntityValue(value).values(i.second.entity);

        for (auto i : list)
//            if (i.entity().type() == Entity::Path)
//            {
//                file = files.value(i.id());
//
//                if (file->info()->isFile())
//                    item->add(new CompositeValuePossibleFileItem(list.at(i), file, item));
//                else
//                    item->add(new CompositeValuePossibleDirItem(list.at(i), file, item));
//            }
//            else
                item->add(new CompositeValueValueItem(i.second, item));
    }
}

Qt::ItemFlags CompositeValueModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void CompositeValueModel::add(const QModelIndex &index, const EntityValue &value)
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

void CompositeValueModel::update(const QModelIndex &index)
{
    emit dataChanged(index, index);
}

IDM_PLUGIN_NS_END
