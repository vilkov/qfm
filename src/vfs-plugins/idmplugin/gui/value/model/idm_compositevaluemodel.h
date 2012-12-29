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
#ifndef IDM_COMPOSITEVALUEMODEL_H_
#define IDM_COMPOSITEVALUEMODEL_H_

#include <QtCore/QMap>
#include <vfs/containers/vfs_snapshot.h>
#include "../../../model/idm_model.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueModel : public Model
{
public:
	typedef CompositeEntityValue::List                    ValueList;
	typedef Container::size_type                          size_type;
	typedef QMap<Database::id_type, const SnapshotItem *> Files;

public:
	CompositeValueModel(const EntityValue::Holder &value, QObject *parent = 0);
	CompositeValueModel(const EntityValue::Holder &value, const Files &files, QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

	void add(const QModelIndex &index, const EntityValue::Holder &value);
	void add(const QModelIndex &index, const ValueList &values);
	void remove(const QModelIndex &index);
    void update(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEMODEL_H_ */
