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
#ifndef IDM_QUERYRESULTROOTITEM_H_
#define IDM_QUERYRESULTROOTITEM_H_

#include <liquiddb/EntityValue>
#include "idm_queryresultitem.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class QueryResultRootItem : public QueryResultItem
{
public:
    QueryResultRootItem(const EntityValue &value, Base *parent = 0);

    /* Base */
    virtual QVariant data(qint32 column, qint32 role) const;
    virtual bool isRoot();
    virtual bool isProperty();
    virtual bool isValue();
    virtual bool isPath();
    virtual bool isRootPathValue();

    const EntityValue &value() const { return m_value; }

private:
    EntityValue m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTROOTITEM_H_ */
