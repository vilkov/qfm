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
#ifndef IDM_CONSTRAINT_H_
#define IDM_CONSTRAINT_H_

#include "../idm_baseconstraint.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class Constraint : public BaseConstraint
{
public:
	enum Operator
	{
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		Like
	};

public:
	Constraint(const Entity::Property &property, Operator op, const EntityValue::Holder &value, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual bool isGroup() const;
	virtual QString toString() const;

	const Entity::Property &property() const { return m_property; }
	Operator op() const { return m_op; }
	const EntityValue::Holder &value() const { return m_value; }

	static QString operatorToString(Operator op);

private:
	static QString operatorToString(Operator op, Database::EntityType type, const QVariant &value);

private:
	Entity::Property m_property;
	Operator m_op;
	EntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CONSTRAINT_H_ */
