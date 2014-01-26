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
#ifndef IDM_ENTITIESTABLE_H_
#define IDM_ENTITIESTABLE_H_

#include "idm_databasetypes.h"


IDM_PLUGIN_NS_BEGIN

struct EntitiesTable
{
	enum Fields
	{
		Id = 0,
		Type = 1,
		Name = 2,
		ShortFormat = 3
	};

	static QString tableName();

	static QByteArray create();
	static QByteArray select();
	static QByteArray select(Database::id_type id);
	static QByteArray insert();
	static QByteArray insertIntoSingle(Database::id_type id);
	static QByteArray insertIntoComposite(Database::id_type id);
	static QByteArray create(Database::EntityType type, Database::id_type id);
	static QByteArray insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat);
	static QByteArray updateEditorGeometry(Database::id_type entity);
	static QByteArray updateListGeometry(Database::id_type entity);
	static QByteArray remove(Database::id_type entity);
	static QByteArray selectValues(Database::id_type entity, Database::id_type property);
	static QByteArray addCompositeValue(Database::id_type entity, Database::id_type id);
	static QByteArray addValue(Database::id_type entity, Database::id_type id);
	static QByteArray updateValue(Database::id_type entity, Database::id_type id);
	static QByteArray removeValue(Database::id_type entity, Database::id_type value);
	static QByteArray removeValues(Database::id_type entity, const Database::IdsList &ids);
	static QByteArray removeProperty(Database::id_type entity, Database::id_type property);
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESTABLE_H_ */
