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
#include "idm_queryresultsnode_performremovetask.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
    ::VFS::Plugins::Default::FilesBaseTask(receiver),
    m_snapshot(snapshot),
    m_progress(receiver),
    m_skipAllIfNotRemove(false)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
    bool tryAgain;
    SnapshotItem *entry;

    for (Snapshot::iterator i = m_snapshot.begin(), end = m_snapshot.end(); i != end && !aborted; ++i)
        if (entry = (*i).second)
            if (entry->info()->isDir())
            {
                m_progress.init((*i).first);
                removeEntry(i.key(), entry, tryAgain = false, aborted);
                m_progress.complete();
            }
            else
            {
                m_progress.clear();
                removeEntry(i.key(), entry, tryAgain = false, aborted);
            }

    postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, m_snapshot));
}

void PerformRemoveTask::removeEntry(const Location &location, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
    if (entry->info()->isDir())
    {
        SnapshotItem *localEntry;

        for (SnapshotItem::iterator i = entry->begin(), end = entry->end(); i != end && !aborted;)
        {
            removeEntry(localEntry = (*i), tryAgain = false, aborted);

            if (localEntry->isRemoved())
                i = entry->erase(i);
            else
            {
                ++i;
                entry->setRemoved(false);
            }
        }

        if (entry->isRemoved())
            do
                doRemove(location, entry, tryAgain = false, aborted);
            while (tryAgain && !aborted);
    }
    else
        do
            doRemove(location, entry, tryAgain = false, aborted);
        while (tryAgain && !aborted);
}

void PerformRemoveTask::doRemove(const Location &location, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
    if (!entry->container()->remove(location, m_error))
        if (m_skipAllIfNotRemove)
            entry->setRemoved(false);
        else
        {
            qint32 answer = askUser(
                                tr("Failed to remove"),
                                tr("\"%1\" (%2). Skip it?").
                                    arg(entry->container()->location(location)).
                                    arg(m_error),
                                QMessageBox::Yes |
                                QMessageBox::YesToAll |
                                QMessageBox::Retry |
                                QMessageBox::Cancel,
                                aborted);

            if (answer == QMessageBox::YesToAll)
            {
                m_skipAllIfNotRemove = true;
                entry->setRemoved(false);
            }
            else
                if (answer == QMessageBox::Retry)
                    tryAgain = true;
                else
                {
                    if (answer == QMessageBox::Cancel)
                        cancel();

                    entry->setRemoved(false);
                }
        }
}

void PerformRemoveTask::removeEntry(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
    if (entry->info()->isDir())
    {
        SnapshotItem *localEntry;

        for (SnapshotItem::iterator i = entry->begin(), end = entry->end(); i != end && !aborted;)
        {
            removeEntry(localEntry = (*i), tryAgain = false, aborted);

            if (localEntry->isRemoved())
                i = entry->erase(i);
            else
            {
                ++i;
                entry->setRemoved(false);
            }
        }

        if (entry->isRemoved())
            do
                doRemove(entry, tryAgain = false, aborted);
            while (tryAgain && !aborted);
    }
    else
        do
            doRemove(entry, tryAgain = false, aborted);
        while (tryAgain && !aborted);
}

void PerformRemoveTask::doRemove(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
    if (!entry->container()->remove(entry->info(), m_error))
        if (m_skipAllIfNotRemove)
            entry->setRemoved(false);
        else
        {
            qint32 answer = askUser(
                                tr("Failed to remove"),
                                tr("\"%1\" (%2). Skip it?").
                                    arg(entry->container()->location(entry->info())).
                                    arg(m_error),
                                QMessageBox::Yes |
                                QMessageBox::YesToAll |
                                QMessageBox::Retry |
                                QMessageBox::Cancel,
                                aborted);

            if (answer == QMessageBox::YesToAll)
            {
                m_skipAllIfNotRemove = true;
                entry->setRemoved(false);
            }
            else
                if (answer == QMessageBox::Retry)
                    tryAgain = true;
                else
                {
                    if (answer == QMessageBox::Cancel)
                        cancel();

                    entry->setRemoved(false);
                }
        }
}

IDM_PLUGIN_NS_END
