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
#ifndef IDM_INODEQUERYRESULTS_H_
#define IDM_INODEQUERYRESULTS_H_

#include <liquiddb/EntityValue>
#include <QtCore/QModelIndex>
#include "../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class IQueryResultsUpdater
{
public:
    virtual ~IQueryResultsUpdater();

    virtual void add(const QModelIndex &index, const EntityValue::List &values) = 0;
    virtual void remove(const QModelIndex &index, const EntityValue::List &values) = 0;
    virtual void refresh(const QModelIndex &index) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDM_INODEQUERYRESULTS_H_ */
