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
#ifndef IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_
#define IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_

#include <vfs/containers/vfs_snapshotitem.h>
#include "idm_compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleDirItem : public CompositeValuePathItem
{
public:
    typedef QList<IdmItem *> Container;

public:
    CompositeValuePossibleDirItem(const EntityValue &value, const SnapshotItem *source, IdmItem *parent = 0);
    virtual ~CompositeValuePossibleDirItem();

    /* Base */
    virtual Base *at(size_type index) const;
    virtual size_type size() const;
    virtual size_type indexOf(Base *item) const;

    /* IdmItem */
    virtual QVariant data(qint32 column, qint32 role) const;

    /* CompositeValuePathItem */
    virtual QString fileName() const;
    virtual bool isFile() const;
    virtual void open() const;

protected:
    friend class CompositeValueModel;
    void add(IdmItem *item) { m_items.push_back(item); }
    void remove(size_type index) { delete m_items.takeAt(index); }
    const SnapshotItem *source() const { return m_source; }

private:
    const SnapshotItem *m_source;
    Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_ */
