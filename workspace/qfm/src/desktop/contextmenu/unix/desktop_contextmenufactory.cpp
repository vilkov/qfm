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
#include "../desktop_contextmenufactory.h"


DESKTOP_NS_BEGIN

ContextMenuFactory::ContextMenuFactory()
{}

void ContextMenuFactory::registerAction(const ::VFS::Action *action, Type type)
{
	m_actions[type].push_back(action);
}

void ContextMenuFactory::registerAction(const ::VFS::Action *action, const FileTypesList &fileTypes)
{
	for (FileTypesList::size_type i = 0, size = fileTypes.size(); i < size; ++i)
		m_files[fileTypes.at(i)].push_back(action);
}

DESKTOP_NS_END
