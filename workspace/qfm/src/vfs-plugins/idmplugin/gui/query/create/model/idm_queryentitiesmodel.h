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
#ifndef IDM_QUERYENTITIESMODEL_H_
#define IDM_QUERYENTITIESMODEL_H_

#include <liquiddb/Entity>
#include "../../../../model/idm_model.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class QueryEntitiesModel : public Model
{
public:
    typedef Container::size_type size_type;

public:
    QueryEntitiesModel(QObject *parent = 0);

    void add(const Entity::Property &property);
    void remove(const QModelIndex &index);

private:
    void expand(IdmItem *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYENTITIESMODEL_H_ */
