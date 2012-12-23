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
#ifndef VFS_CONTEXTMENU_H_
#define VFS_CONTEXTMENU_H_

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QCoreApplication>
#include <tools/containers/orderedmap.h>
#include "../actions/vfs_fileaction.h"


VFS_NS_BEGIN

class ContextMenu
{
    Q_DECLARE_TR_FUNCTIONS(ContextMenu)

public:
    enum Section
    {
        GeneralSection,
        ActionsSection,
        OpenWithSection,
        MiscStuffSection,
        PropertiesSection
    };

public:
    ContextMenu(const IFileContainer *container);
    ~ContextMenu();

    FileAction::FilesList files(const FileAction *action) const;

    void add(const FileAction *action, Section section);
    void add(const Item::Holder &item, const IFileInfo *info);
    void add(const FileAction *action, const FileAction::FilesList &files, Section section);

    const FileAction *exec();

private:
    const IFileContainer *m_container;

private:
    typedef QMap<const FileAction *, FileAction::FilesList> Contaier;
    Contaier m_files;

private:
    typedef QList<const FileAction *> FileActions;
    FileActions m_actions[PropertiesSection + 1];

private:
    typedef QMap<FileTypeId, FileAction::FilesList> FilesByTypeMap;
    FilesByTypeMap m_filesByType;
    FileAction::FilesList m_allFiles;

private:
    typedef QMap<const IApplication *, FileAction *> OpenWithActionsMap;
    OpenWithActionsMap m_openWithActions;

private:
    typedef ::Tools::Containers::OrderedMap<const FileAction *, FileAction::FilesList> OrderedActionsMap;
    OrderedActionsMap m_mapOpenWithActions;
};

VFS_NS_END

#endif /* VFS_CONTEXTMENU_H_ */
