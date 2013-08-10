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
#include "default_performcopytask.h"
#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesBaseTask(receiver, destination),
	m_move(move),
	m_snapshot(snapshot),
    m_tryier(NULL),
    m_questioner(NULL),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{
    Tryier tryier(this, &PerformCopyTask::askForSkipIfNotCopy, aborted);
    Questioner questioner(this, &PerformCopyTask::askForOverwrite, aborted);

    m_tryier = &tryier;
    m_questioner = &questioner;

    for (Snapshot::const_iterator it = m_snapshot.begin(), end = m_snapshot.end(); it != end && !aborted; ++it)
	{
		m_progress.init((*it).first);
		copyEntry(destination().data(), (*it).second, aborted);
		m_progress.complete();
	}

	postEvent(new ExtendedEvent(this, Event::CopyFiles, destination(), aborted, m_snapshot, m_move));
}

bool PerformCopyTask::CreateDestinationFolder::operator()(QString &error) const
{
    return m_result = m_container->create(m_fileName, error);
}

bool PerformCopyTask::OverwriteFile::operator()() const
{
    return !m_destination->contains(m_fileName);
}

void PerformCopyTask::OverwriteFile::operator()(QString &error) const
{
    error = tr("File \"%1\" from \"%2\" already exists in \"%3\".").
            arg(m_fileName).
            arg(m_source->location()).
            arg(m_destination->location()).
            append(QChar(L'\n')).
            append(tr("Overwrite it?"));
}

bool PerformCopyTask::OpenSourceFile::operator()(QString &error) const
{
    return m_result = m_container->open(file, IFileAccessor::ReadOnly, error);
}

bool PerformCopyTask::CreateDestinationFile::operator()(QString &error) const
{
    return m_result = m_container->create(file->fileName(), IFileAccessor::ReadWrite | IFileAccessor::Truncate, error);
}

void PerformCopyTask::copyEntry(const IFileContainer *destination, SnapshotItem *entry, const volatile Flags &aborted)
{
    if (entry->info()->isDir())
    {
        ::Tools::Memory::ScopedPointer<IFileContainer> dest;

        if (m_tryier->tryTo(CreateDestinationFolder(destination, entry->info()->fileName(), dest)))
            for (SnapshotItem::const_iterator i = entry->begin(), end = entry->end(); i != end && !aborted; ++i)
            {
                copyEntry(dest.data(), (*i), aborted);

                if (!(*i)->isRemoved())
                    entry->setRemoved(false);
            }
        else
            entry->setRemoved(false);
    }
    else
        if (m_questioner->askFor(OverwriteFile(entry->container(), destination, entry->info()->fileName())))
            copyFile(destination, entry, aborted);
        else
            entry->setRemoved(false);
}

void PerformCopyTask::copyFile(const IFileContainer *destination, SnapshotItem *entry, const volatile Flags &aborted)
{
    Tryier::Scope scope(m_tryier);

    do
        if (m_tryier->tryTo(OpenSourceFile(entry->container(), entry->info(), m_sourceFile)) &&
            m_tryier->tryTo(CreateDestinationFile(destination, entry->info(), m_destFile)))
        {
            m_destFile->setPermissions(m_sourceFile->permissions());
            m_written = 0;

            while ((m_readed = m_sourceFile->read(m_buffer, FileReadWriteGranularity)) && !aborted)
                if (m_destFile->write(m_buffer, m_readed) == m_readed)
                {
                    m_written += m_readed;
                    m_progress.update(m_readed);
                }
                else
                {
                    m_tryier->failed(tr("Failed to write to file \"%1\" (%2).").
                                        arg(destination->location(entry->info()->fileName())).
                                        arg(m_destFile->lastError()));
                    break;
                }
        }
        else
            entry->setRemoved(m_tryier->tryAgain());
    while (m_tryier->tryAgain());
}

bool PerformCopyTask::askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Task::Flags &aborted)
{
    qint32 answer = askUser(
                        tr("Failed to copy..."),
                        QString(error).append(QChar(L'\n')).append(tr("Skip it?")),
                        QMessageBox::Yes |
                        QMessageBox::YesToAll |
                        QMessageBox::Retry |
                        QMessageBox::Cancel,
                        aborted);

    switch (answer)
    {
        case QMessageBox::Yes:
            return false;

        case QMessageBox::YesToAll:
            return flag = false;

        case QMessageBox::Retry:
            return true;

        case QMessageBox::Cancel:
            cancel();
            break;
    }

    return false;
}

bool PerformCopyTask::askForOverwrite(const QString &error, Questioner::Tristate &flag, const volatile Task::Flags &aborted)
{
    qint32 answer = askUser(
                        tr("Failed to copy..."),
                        error,
                        QMessageBox::Yes |
                        QMessageBox::YesToAll |
                        QMessageBox::No |
                        QMessageBox::NoToAll |
                        QMessageBox::Cancel,
                        aborted);

    switch (answer)
    {
        case QMessageBox::Yes:
            return true;

        case QMessageBox::YesToAll:
            return flag = true;

        case QMessageBox::No:
            return false;

        case QMessageBox::NoToAll:
            return flag = false;

        case QMessageBox::Cancel:
            cancel();
            break;
    }

    return false;
}

DEFAULT_PLUGIN_NS_END
