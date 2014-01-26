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
#ifndef IDM_SELECTQUERY_H_
#define IDM_SELECTQUERY_H_

#include "idm_query.h"
#include "../constraints/idm_baseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	Select(Entity *entity);
	Select(Entity *entity, BaseConstraint *where);

	virtual QByteArray compile() const;

private:
	struct Format
	{
		Format();

		QString select(Entity *entity) const;
		QString join(Database::id_type entity, Database::id_type property) const;
		QString complete(Database::id_type entity, QString &selectedFields, QString &joinedFields, const QString &where) const;

		QString format;
		QString format2;
		QString idField;
		QString valueField;
	};

	void join(const Format &format, QString &selectedFields, QString &joinedFields, Entity *entity, Entity *property) const;

private:
	BaseConstraint::Holder m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDM_SELECTQUERY_H_ */
