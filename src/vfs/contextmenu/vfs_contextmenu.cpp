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
#include "vfs_contextmenu.h"
#include "actions/vfs_openwithaction.h"

#include <application.h>
#include <QtGui/QMenu>
#include <QtGui/QCursor>


VFS_NS_BEGIN

ContextMenu::ContextMenu(const IFileContainer *container) :
    m_container(container)
{
    Q_ASSERT(m_container);
}

ContextMenu::~ContextMenu()
{
    qDeleteAll(m_openWithActions);
}

Action::FilesList ContextMenu::files(const Action *action) const
{
    return m_files.value(action);
}

void ContextMenu::add(const Action *action, Section section)
{
    m_actions[section].push_back(action);
}

void ContextMenu::add(const Item::Holder &item, const IFileInfo *info)
{
    m_allFiles.push_back(Action::FilesList::value_type(item, info));
    m_filesByType[info->fileType()->id()].push_back(Action::FilesList::value_type(item, info));

    IApplications::LinkedList applications = m_container->applications()->user(info->fileType());

    if (applications.isEmpty())
        applications = m_container->applications()->system(info->fileType());

    for (IApplications::LinkedList::const_iterator i = applications.begin(), end = applications.end(); i != end; ++i)
        if (Action *&action = m_openWithActions[*i])
            m_mapOpenWithActions[action].push_back(Action::FilesList::value_type(item, info));
        else
        {
            action = new OpenWithAction(*i);
            m_mapOpenWithActions[action].push_back(Action::FilesList::value_type(item, info));
        }
}

void ContextMenu::add(const Action *action, const Action::FilesList &files, Section section)
{
    m_actions[section].push_back(action);
    m_files[action] = files;
}

const Action *ContextMenu::exec()
{
    typedef ::VFS::Actions::FileActionList FileActionsList;
    FileActionsList actions;

    QMenu menu;
    QMenu openWithMenu(tr("Open with"));

    for (FilesByTypeMap::const_iterator i = m_filesByType.begin(), end = m_filesByType.end(); i != end; ++i)
    {
        const Actions::Array actionsTypes = Application::globalActions()->actions(i.key());

        if ((*i).size() == 1)
            actions = actionsTypes[Actions::ForSingleFile];
        else
            actions = actionsTypes[Actions::ForMultipleFiles];

        for (FileActionsList::size_type q = 0, size = actions.size(); q < size; ++q)
            add(actions.at(q), (*i), ContextMenu::ActionsSection);

        actions = actionsTypes[Actions::ForOneOrMoreFiles];

        for (FileActionsList::size_type q = 0, size = actions.size(); q < size; ++q)
            add(actions.at(q), (*i), ContextMenu::ActionsSection);
    }

    for (FileActions::const_iterator i = m_actions[GeneralSection].constBegin(), end = m_actions[GeneralSection].constEnd(); i != end; ++i)
        menu.addAction(const_cast<QAction *>((*i)->action()));

    for (Section section = ActionsSection; section <= PropertiesSection; ++reinterpret_cast<int &>(section))
    {
        if (!m_actions[section].isEmpty())
        {
            menu.addSeparator();

            for (FileActions::const_iterator i = m_actions[section].constBegin(), end = m_actions[section].constEnd(); i != end; ++i)
                menu.addAction(const_cast<QAction *>((*i)->action()));
        }

        if (section == OpenWithSection && !m_mapOpenWithActions.isEmpty())
        {
            if (m_actions[section].isEmpty())
                menu.addSeparator();

            for (OrderedActionsMap::const_iterator i = m_mapOpenWithActions.begin(), end = m_mapOpenWithActions.end(); i != end; ++i)
            {
                m_files[i.key()] = *i;
                openWithMenu.addAction(const_cast<QAction *>(i.key()->action()));
            }

            menu.addMenu(&openWithMenu);
        }
    }

    Action *res = Action::fromQAction(menu.exec(QCursor::pos()));

    openWithMenu.clear();
    menu.clear();

    return res;
}

VFS_NS_END
