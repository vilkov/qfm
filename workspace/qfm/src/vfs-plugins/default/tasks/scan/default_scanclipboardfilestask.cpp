/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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
#include "default_scanclipboardfilestask.h"

#include <vfs/containers/vfs_selfcontainedsnapshotitem.h>
#include <application.h>


DEFAULT_PLUGIN_NS_BEGIN

ScanClipboardFilesTask::ScanClipboardFilesTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &files, bool move) :
    FilesBaseTask(receiver, destination),
    m_move(move),
    m_files(files)
{}

void ScanClipboardFilesTask::run(const volatile Flags &aborted)
{
    IFileInfo::Holder info;
    IFileContainer::Holder container;
    QString error;

    for (Snapshot::iterator i = m_files.begin(), end = m_files.end(); i != end && !aborted; ++i)
    {
        Application::rootNode()->container(i.key(), container, info, error);

        if (container && info)
            m_files.insert(i, new SelfContainedSnapshotItem(container, info, NULL));
    }

    postEvent(new CopyEvent(this, destination(), aborted, m_files, m_move));
}

DEFAULT_PLUGIN_NS_END
