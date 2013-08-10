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
#include "../actions/vfs_action.h"


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

    Action::FilesList files(const Action *action) const;

    void add(const Action *action, Section section);
    void add(const Item::Holder &item, const IFileInfo *info);
    void add(const Action *action, const Action::FilesList &files, Section section);

    const Action *exec();

private:
    const IFileContainer *m_container;

private:
    typedef QMap<const Action *, Action::FilesList> Contaier;
    Contaier m_files;

private:
    typedef QList<const Action *> FileActions;
    FileActions m_actions[PropertiesSection + 1];

private:
    typedef QMap<FileTypeId, Action::FilesList> FilesByTypeMap;
    FilesByTypeMap m_filesByType;
    Action::FilesList m_allFiles;

private:
    typedef QMap<const IApplication *, Action *> OpenWithActionsMap;
    OpenWithActionsMap m_openWithActions;

private:
    typedef ::Tools::Containers::OrderedMap<const Action *, Action::FilesList> OrderedActionsMap;
    OrderedActionsMap m_mapOpenWithActions;
};

VFS_NS_END

#endif /* VFS_CONTEXTMENU_H_ */
