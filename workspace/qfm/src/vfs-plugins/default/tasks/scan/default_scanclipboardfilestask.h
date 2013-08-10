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
#ifndef DEFAULT_SCANCLIPBOARDFILESTASK_H_
#define DEFAULT_SCANCLIPBOARDFILESTASK_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "../default_filesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class ScanClipboardFilesTask : public FilesBaseTask
{
public:
    class CopyEvent : public ExtendedEvent
    {
    public:
        CopyEvent(BaseTask *task, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
            ExtendedEvent(task, FilesBaseTask::Event::ScanClipboardFiles_Update, destination, canceled, snapshot),
            move(move)
        {}

        bool move;
    };

public:
    ScanClipboardFilesTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &files, bool move);

protected:
    virtual void run(const volatile Flags &aborted);

private:
    bool m_move;
    Snapshot m_files;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_SCANCLIPBOARDFILESTASK_H_ */
