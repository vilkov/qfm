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
#include "idm_model.h"


IDM_PLUGIN_NS_BEGIN

Model::Model(QObject *parent) :
    ::Tools::Models::Tree::Model(m_container, parent),
    m_items(m_container.m_container)
{}

Model::Container::Container()
{}

Model::Container::~Container()
{
    qDeleteAll(m_container);
}

Model::Container::size_type Model::Container::size() const
{
    return m_container.size();
}

Model::Container::Item *Model::Container::at(size_type index) const
{
    return m_container.at(index);
}

Model::Container::size_type Model::Container::indexOf(Item *item) const
{
    return m_container.indexOf(item);
}

IDM_PLUGIN_NS_END
