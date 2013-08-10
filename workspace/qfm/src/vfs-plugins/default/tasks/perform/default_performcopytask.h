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
#ifndef DEFAULT_PERFORMCOPYTASK_H_
#define DEFAULT_PERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include <tools/taskspool/tryier.h>
#include <tools/taskspool/questioner.h>
#include "../default_filesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformCopyTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

public:
	class ExtendedEvent : public FilesBaseTask::ExtendedEvent
	{
	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			FilesBaseTask::ExtendedEvent(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

protected:
    typedef ::Tools::TasksPool::Tryier<PerformCopyTask>     Tryier;
    typedef ::Tools::TasksPool::Questioner<PerformCopyTask> Questioner;

    class CreateDestinationFolder
    {
    public:
        CreateDestinationFolder(const IFileContainer *container, const QString &fileName, IFileContainer::Holder &result) :
            m_container(container),
            m_fileName(fileName),
            m_result(result)
        {}

        inline bool operator()(QString &error) const;

    private:
        const IFileContainer *m_container;
        const QString &m_fileName;
        IFileContainer::Holder &m_result;
    };

    class OverwriteFile
    {
        Q_DECLARE_TR_FUNCTIONS(PerformCopyTask::OverwriteFile)

    public:
        OverwriteFile(const IFileContainer *source, const IFileContainer *destination, const QString &fileName) :
            m_source(source),
            m_destination(destination),
            m_fileName(fileName)
        {}

        inline bool operator()() const;
        inline void operator()(QString &error) const;

    private:
        const IFileContainer *m_source;
        const IFileContainer *m_destination;
        const QString &m_fileName;
    };

    class OpenSourceFile
    {
    public:
        OpenSourceFile(const IFileContainer *container, const IFileInfo *file, ::Tools::Memory::ScopedPointer<IFileAccessor> &result) :
            m_container(container),
            file(file),
            m_result(result)
        {}

        inline bool operator()(QString &error) const;

    private:
        const IFileContainer *m_container;
        const IFileInfo *file;
        ::Tools::Memory::ScopedPointer<IFileAccessor> &m_result;
    };

    class CreateDestinationFile
    {
    public:
        CreateDestinationFile(const IFileContainer *container, const IFileInfo *file, ::Tools::Memory::ScopedPointer<IFileAccessor> &result) :
            m_container(container),
            file(file),
            m_result(result)
        {}

        inline bool operator()(QString &error) const;

    private:
        const IFileContainer *m_container;
        const IFileInfo *file;
        ::Tools::Memory::ScopedPointer<IFileAccessor> &m_result;
    };

protected:
    void copyEntry(const IFileContainer *destination, SnapshotItem *entry, const volatile Flags &aborted);
    virtual void copyFile(const IFileContainer *destination, SnapshotItem *entry, const volatile Flags &aborted);

    bool askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Task::Flags &aborted);
    bool askForOverwrite(const QString &error, Questioner::Tristate &flag, const volatile Task::Flags &aborted);

protected:
    bool m_move;
    Snapshot m_snapshot;

    Tryier *m_tryier;
    Questioner *m_questioner;
	TaskProgress m_progress;

	::Tools::Memory::ScopedPointer<IFileAccessor> m_destFile;
	::Tools::Memory::ScopedPointer<IFileAccessor> m_sourceFile;
	IFileAccessor::size_type m_readed;
	IFileAccessor::size_type m_written;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PERFORMCOPYTASK_H_ */
