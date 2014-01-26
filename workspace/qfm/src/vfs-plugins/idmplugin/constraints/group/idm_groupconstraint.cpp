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
#include "idm_groupconstraint.h"


IDM_PLUGIN_NS_BEGIN

GroupConstraint::GroupConstraint(LiquidDb::GroupConstraint::Type type, BaseConstraint *parent) :
	BaseConstraint(parent),
    m_constraint(type)
{}

bool GroupConstraint::isGroup() const
{
	return true;
}

const LiquidDb::Constraint &GroupConstraint::constraint() const
{
    return m_constraint;
}

GroupConstraint::Container::size_type GroupConstraint::indexOf(BaseConstraint *item) const
{
    for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
        if (m_items[i].data() == item)
            return i;

    return InvalidIndex;
}

void GroupConstraint::add(const BaseConstraint::Holder &constraint)
{
	m_items.push_back(constraint);
	m_constraint.add(constraint->constraint());
}

void GroupConstraint::insert(Container::size_type index, const BaseConstraint::Holder &constraint)
{
//	m_items.insert(index, constraint);
}

void GroupConstraint::swap(Container::size_type index1, Container::size_type index2)
{
//	m_items.swap(index1, index2);
}

void GroupConstraint::remove(Container::size_type index)
{
//	m_items.takeAt(index);
}

IDM_PLUGIN_NS_END
