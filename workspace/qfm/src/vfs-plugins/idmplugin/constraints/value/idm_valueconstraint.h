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
#ifndef IDM_VALUECONSTRAINT_H_
#define IDM_VALUECONSTRAINT_H_

#include "../idm_baseconstraint.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class ValueConstraint : public BaseConstraint
{
public:
    ValueConstraint(const Entity::Property &property, Constraint::Operator op, const EntityValue &value, BaseConstraint *parent = 0);

	/* BaseConstraint */
	virtual bool isGroup() const;
    virtual const Constraint &constraint() const;

	const Entity::Property &property() const { return m_property; }
	Constraint::Operator op() const { return m_constraint.op(); }
	const EntityValue &value() const { return m_constraint.value(); }

private:
	Entity::Property m_property;
	EntityConstraint m_constraint;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUECONSTRAINT_H_ */
