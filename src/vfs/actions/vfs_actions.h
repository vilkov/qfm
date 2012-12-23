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
#ifndef VFS_ACTIONS_H_
#define VFS_ACTIONS_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "vfs_action.h"
#include "../filetypeinfo/vfs_filetypeinfo.h"


VFS_NS_BEGIN

class Actions
{
public:
    typedef QList<FileTypeId>         FileTypeList;
    typedef QList<const Action *> FileActionList;

    enum ActionsType
    {
        ForSingleFile,
        ForMultipleFiles,
        ForOneOrMoreFiles
    };

    struct Array
    {
        const FileActionList &operator[](ActionsType actionsType) const { return m_type[actionsType]; }
        FileActionList &operator[](ActionsType actionsType) { return m_type[actionsType]; }

    private:
        FileActionList m_type[ForOneOrMoreFiles + 1];
    };

public:
    Actions();

    Array actions(const FileTypeId &fileType) const { return m_actions.value(fileType); }
    FileActionList actions(const FileTypeId &fileType, ActionsType type) const { return m_actions.value(fileType)[type]; }
    void registerAction(const Action *action, const FileTypeList &filesTypes, ActionsType type = ForOneOrMoreFiles);
    void registerAction(const Action *action, const FileTypeId &fileType, ActionsType type = ForOneOrMoreFiles);

private:
    typedef QMap<FileTypeId, Array> FileActionMap;

private:
    FileActionMap m_actions;
};

VFS_NS_END

#endif /* VFS_ACTIONS_H_ */
