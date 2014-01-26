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
#ifndef IDM_QUERYRESULTITEM_H_
#define IDM_QUERYRESULTITEM_H_

#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultItem : public TasksNodeItem
{
public:
    QueryResultItem(Base *parent);

    virtual bool isRoot() = 0;
    virtual bool isProperty() = 0;
    virtual bool isValue() = 0;
    virtual bool isPath() = 0;
    virtual bool isRootPathValue() = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTITEM_H_ */
