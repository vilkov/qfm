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
#ifndef IDM_DATABASETYPES_H_
#define IDM_DATABASETYPES_H_

#include <QtCore/QSet>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../../idm_ns.h"


typedef struct sqlite3_stmt sqlite3_stmt;


IDM_PLUGIN_NS_BEGIN

struct Database
{
	enum EntityType
	{
		Int        = 1,
		String     = 2,
		Date       = 3,
		Time       = 4,
		DateTime   = 5,
		Memo       = 6,
		Composite  = 7,
		Rating     = 8,
		Path       = 9
	};

	typedef unsigned int id_type;
	enum { InvalidId = (id_type)-1 };

	typedef QSet<id_type>  IdsSet;
	typedef QList<id_type> IdsList;

	static QByteArray init();
	static QByteArray loadId(const QString &tableName);
	static QByteArray savepoint(const QByteArray &baseName);
	static QString typeToString(EntityType type);
	static QString valueToString(EntityType type, const QVariant &value);
	static QString valueToConstraintString(EntityType type, const QVariant &value);
	static QString idsToString(const IdsSet &ids);
	static QString idsToString(const IdsList &ids);
	static bool bind(EntityType type, sqlite3_stmt *source, int sCol, sqlite3_stmt *dest, int dCol, QByteArray &buffer);
};

IDM_PLUGIN_NS_END

#endif /* IDM_DATABASETYPES_H_ */
