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
#ifndef IDM_COMPOSITEVALUEPATHITEM_H_
#define IDM_COMPOSITEVALUEPATHITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class CompositeValuePathItem : public CompositeValueItem
{
public:
    CompositeValuePathItem(const EntityValue &value, IdmItem *parent = 0);

    /* CompositeValueItem */
    virtual bool isPath() const;

    virtual QString fileName() const = 0;
    virtual bool isFile() const = 0;
    virtual void open() const = 0;

    const EntityValue &value() const { return m_value; }
    EntityValue take() { EntityValue res(m_value); m_value = EntityValue(); return res; }

protected:
    EntityValue m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPATHITEM_H_ */
