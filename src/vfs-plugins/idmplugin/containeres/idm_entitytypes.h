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
#ifndef IDM_ENTITYTYPES_H_
#define IDM_ENTITYTYPES_H_

#include <QtCore/QMap>
#include "../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

struct EntityTypeDescription
{
	EntityTypeDescription()
	{}
	EntityTypeDescription(const QString &label, const QString &description) :
		label(label),
		description(description)
	{}

	QString label;
	QString description;
};

typedef QMap<Entity::Type, EntityTypeDescription> EntityTypes;

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITYTYPES_H_ */
