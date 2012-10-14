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
#include "idm_container.h"
#include "../idm_plugin.h"
#include "../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmContainer::IdmContainer(IFileContainer::Holder &container, bool create) :
	m_data(new Data(container, create))
{
	m_data->menuActions.push_back(new QAction(tr("Create"), 0));
	m_data->menuActions.last()->setData(Create);

	m_data->menuActions.push_back(new QAction(tr("Find"), 0));
	m_data->menuActions.last()->setData(Find);

	m_data->menuActions.push_back(new QAction(tr("List"), 0));
	m_data->menuActions.last()->setData(List);

	m_data->entityTypes[Database::Int]       = EntityTypeDescription(tr("Int"),       tr("Integer type"));
	m_data->entityTypes[Database::String]    = EntityTypeDescription(tr("String"),    tr("String type (max length is 256 characters)"));
	m_data->entityTypes[Database::Date]      = EntityTypeDescription(tr("Date"),      tr("Date type"));
	m_data->entityTypes[Database::Time]      = EntityTypeDescription(tr("Time"),      tr("Time type"));
	m_data->entityTypes[Database::DateTime]  = EntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
	m_data->entityTypes[Database::Memo]      = EntityTypeDescription(tr("Memo"),      tr("Memo type"));
	m_data->entityTypes[Database::Composite] = EntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
	m_data->entityTypes[Database::Rating]    = EntityTypeDescription(tr("Rating"),    tr("Rating type (shown as stars)"));
	m_data->entityTypes[Database::Path]      = EntityTypeDescription(tr("Path"),      tr("Path to file or folder"));
}

IdmContainer::Data::Data(IFileContainer::Holder &container, bool create) :
	storage(container->location(Plugin::fileName()), create),
	container(container.take())
{}

IdmContainer::Data::~Data()
{
	qDeleteAll(menuActions);
}

IDM_PLUGIN_NS_END
