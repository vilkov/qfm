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
#ifndef IDM_ENTITIESLISTMODEL_H_
#define IDM_ENTITIESLISTMODEL_H_

#include <liquiddb/Entity>
#include "../../../model/idm_model.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class EntitiesListModel : public Model
{
public:
    typedef Container::size_type size_type;

public:
    EntitiesListModel(QObject *parent = 0);

    /* Model */
    virtual int columnCount(const QModelIndex &parent) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    const Entity &entityAt(size_type index) const;
    const ::EFC::String &nameAt(size_type index) const;
    size_type size() const { return m_items.size(); }

    void add(const Entity &entity, const ::EFC::String &name);
    void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESLISTMODEL_H_ */
