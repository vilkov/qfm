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
#include "treemodelitem.h"


MODELS_TREE_NS_BEGIN

Item::Item(Item *parent) :
	m_parent(parent)
{}

Item::~Item()
{}

Item::Base *Item::at(size_type index) const
{
	return NULL;
}

Item::size_type Item::size() const
{
	return 0;
}

Item::size_type Item::indexOf(Base *item) const
{
	return InvalidIndex;
}

MODELS_TREE_NS_END
