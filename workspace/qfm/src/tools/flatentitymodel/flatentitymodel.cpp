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
#include "flatentitymodel.h"


FlatEntityModel::FlatEntityModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

FlatEntityModel::~FlatEntityModel()
{
	qDeleteAll(m_entity);
}

int FlatEntityModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return static_cast<EntityItem*>(parent.internalPointer())->size();
	else
		return m_entity.size();
}

int FlatEntityModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant FlatEntityModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<EntityItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_entity[index.row()]->data(index.column(), role);
}

Qt::ItemFlags FlatEntityModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant FlatEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex FlatEntityModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid())
			return index(row, column, static_cast<EntityItem*>(parent.internalPointer()));
		else
			return createIndex(row, column, m_entity[row]);
    else
        return QModelIndex();
}

QModelIndex FlatEntityModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
    {
    	EntityItem *parentItem = static_cast<EntityItem*>(child.internalPointer())->parent();

		if (parentItem)
			if (parentItem->parent())
				return createIndex(parentItem->parent()->indexOf(parentItem), 0, parentItem);
			else
				return createIndex(m_entity.indexOf(parentItem), 0, parentItem);
    }

    return QModelIndex();
}

void FlatEntityModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, m_entity.size() - 1);
	qDeleteAll(m_entity);
	m_entity.clear();
	endRemoveRows();
}

void FlatEntityModel::remove(const QModelIndex &item)
{
	EntityItem *entityItem = static_cast<EntityItem*>(item.internalPointer());

	if (entityItem->parent() == 0)
	{
		beginRemoveRows(QModelIndex(), item.row(), item.row());
		m_entity.removeAt(item.row());
		delete entityItem;
		endRemoveRows();
	}
}

void FlatEntityModel::remove(const QModelIndexList &items)
{
	qint32 index;
	EntityItem *item;
	QSet<EntityItem*> done;

	for (QModelIndexList::size_type i = 0, size = items.size(); i < size; ++i)
		if (!done.contains(item = static_cast<EntityItem*>(items.at(i).internalPointer())))
		{
			done.insert(item);

			if (item->parent() == 0)
			{
				index = m_entity.indexOf(item);
				beginRemoveRows(QModelIndex(), index, index);
				m_entity.removeAt(index);
				delete item;
				endRemoveRows();
			}
		}
}

QModelIndex FlatEntityModel::index(int column, EntityItem *item) const
{
	int index = m_entity.indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex FlatEntityModel::index(int row, int column, EntityItem *parentItem) const
{
	EntityItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
