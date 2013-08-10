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
#ifndef IDM_PROPERTIESTABLE_H_
#define IDM_PROPERTIESTABLE_H_

#include "idm_databasetypes.h"


IDM_PLUGIN_NS_BEGIN

struct PropertiesTable
{
	enum Fields
	{
		Id = 0,
		EntityId = 1,
		EntityPropertyId = 2,
		Name = 3
	};

	static QString tableName();

	static QByteArray create();
	static QByteArray create(Database::id_type entity, Database::id_type property);
	static QByteArray select();
	static QByteArray select(Database::id_type entity);
	static QByteArray select(Database::id_type entity, Database::id_type property);
	static QByteArray insert();
	static QByteArray insert(Database::id_type id, Database::id_type entity, Database::id_type property, const QString &name);
	static QByteArray insert(Database::id_type entity, Database::id_type property);
	static QByteArray rename(Database::id_type entity, Database::id_type property, const QString &name);
	static QByteArray remove(Database::id_type entity, Database::id_type property);
	static QByteArray addValue(const QString &tableName, Database::id_type id, Database::id_type entity, Database::id_type property);
	static QByteArray removeValue(Database::id_type entity, Database::id_type entityValue, Database::id_type property, Database::id_type propertyValue);
	static QByteArray selectValuesExcept(Database::id_type entity, const Database::IdsList &entityIds, Database::id_type property, const Database::IdsSet &propertyIds);

	struct Incomplete
	{
		static QString selectValues(Database::id_type property);
		static QString selectValues(Database::id_type entity, const Database::IdsList &ids);
		static QString dropProperty(Database::id_type property);
		static QString dropProperty2(Database::id_type property);
		static QByteArray addValue(const QString &tableName, Database::id_type entity);
		static QString removeValues(Database::id_type property, const Database::IdsList &ids);
	};

	struct Parameters
	{
		static QByteArray addValue(Database::id_type entity, Database::id_type property, Database::id_type value);
	};
};

IDM_PLUGIN_NS_END

#endif /* IDM_PROPERTIESTABLE_H_ */
