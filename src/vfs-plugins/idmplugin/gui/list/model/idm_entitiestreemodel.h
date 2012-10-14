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
#ifndef IDM_ENTITIESTREEMODEL_H_
#define IDM_ENTITIESTREEMODEL_H_

#include "../../../model/idm_model.h"
#include "../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesTreeModel : public Model
{
public:
	EntitiesTreeModel(QObject *parent = 0);

	void add(Entity *entity);
	void remove(const QModelIndex &index);

private:
	void doAdd(Entity *entity);
	void doAdd(IdmItem *item, Entity *property);
	void doRemove(IdmItem *item, Container::size_type index);
	void expand(IdmItem *parent);

private:
	typedef QMap<Entity*, Container::List> Map;
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESTREEMODEL_H_ */
