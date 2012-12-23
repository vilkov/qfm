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
#include "vfs_actions.h"


VFS_NS_BEGIN

Actions::Actions()
{}

void Actions::registerAction(const Action *action, const FileTypeList &filesTypes, ActionsType type)
{
    for (FileTypeList::size_type i = 0, size = filesTypes.size(); i < size; ++i)
        m_actions[filesTypes.at(i)][type].push_back(action);
}

void Actions::registerAction(const Action *action, const FileTypeId &fileType, ActionsType type)
{
    m_actions[fileType][type].push_back(action);
}

VFS_NS_END
