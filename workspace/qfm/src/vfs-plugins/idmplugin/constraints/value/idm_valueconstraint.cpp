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
#include "idm_valueconstraint.h"


IDM_PLUGIN_NS_BEGIN

ValueConstraint::ValueConstraint(const Entity::Property &property, Constraint::Operator op, const EntityValue &value, BaseConstraint *parent) :
	BaseConstraint(parent),
	m_property(property),
    m_constraint(m_property.entity, op, value)
{}

bool ValueConstraint::isGroup() const
{
	return false;
}

const Constraint &ValueConstraint::constraint() const
{
    return m_constraint;
}

IDM_PLUGIN_NS_END
