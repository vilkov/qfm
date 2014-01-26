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
#include "idm_entitieslistmodel.h"
#include "items/idm_entitieslistmodelitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesListModel::EntitiesListModel(QObject *parent) :
    Model(parent)
{}

int EntitiesListModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

Qt::ItemFlags EntitiesListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

const Entity &EntitiesListModel::entityAt(size_type index) const
{
    return static_cast<IdmEntitiesListItem *>(m_items.at(index))->entity();
}

const ::EFC::String &EntitiesListModel::nameAt(size_type index) const
{
    return static_cast<IdmEntitiesListItem *>(m_items.at(index))->property().name;
}

void EntitiesListModel::add(const Entity &entity, const ::EFC::String &name)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back(new IdmEntitiesListItem(Entity::Property(entity, name)));
    endInsertRows();
}

void EntitiesListModel::remove(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    delete m_items.takeAt(index.row());
    endRemoveRows();
}

IDM_PLUGIN_NS_END
