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
#include "idm_valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

ValueListModel::ValueListModel(const IdmContainer &container, const Select &query, QObject *parent) :
	QAbstractItemModel(parent),
	m_reader(container, query)
{}

int ValueListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return m_items.size();
}

int ValueListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ValueListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		return m_items.at(index.row())->value();
	else
		return QVariant();
}

Qt::ItemFlags ValueListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void ValueListModel::fetchMore(const QModelIndex &parent)
{
	List list;
	EntityValue::Holder item;

	list.reserve(PrefetchLimit);

	for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
		if (item = m_reader.next())
			list.push_back(item);
		else
			break;

	if (!list.isEmpty())
		add(list);
}

bool ValueListModel::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

QModelIndex ValueListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex ValueListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

EntityValue::Holder ValueListModel::take(const QModelIndex &index)
{
	EntityValue::Holder res;

	beginRemoveRows(QModelIndex(), index.row(), index.row());
	res = m_items.takeAt(index.row());
	endRemoveRows();

	return res;
}

void ValueListModel::add(const List &list)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + list.size() - 1);
	m_items.append(list);
	endInsertRows();
}

QModelIndex ValueListModel::add(const EntityValue::Holder &value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(value);
	endInsertRows();

	return createIndex(m_items.size() - 1, 0);
}

void ValueListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.removeAt(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
