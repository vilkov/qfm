#include "performcopytask.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(TasksNode *receiver, const Snapshot &snapshot, IFileContainer::Holder &destination, bool move) :
	PerformCopyBaseTask(receiver, snapshot, destination),
	m_move(move)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = copy(aborted);
	postEvent(new Event(this, aborted, snapshot, destination(), m_move));
}

void PerformCopyTask::copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (m_sourceFile = source->open(entry->fileName(), IFileAccessor::ReadOnly, m_lastError))
			if (m_destFile = destination->open(entry->fileName(), IFileAccessor::ReadWrite | IFileAccessor::Create | IFileAccessor::Truncate, m_lastError))
			{
				m_written = 0;

				while ((m_readed = m_sourceFile->read(m_buffer, FileReadWriteGranularity)) && !aborted)
					if (m_destFile->write(m_buffer, m_readed) == m_readed)
					{
						m_written += m_readed;
						m_progress.update(m_readed);
					}
					else
					{
						m_lastError = m_destFile->lastError();

						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to write to file \"%1\" (%2). Skip it?").
									arg(destination->location(entry->fileName())).
									arg(m_lastError),
								tryAgain = false,
								aborted);

						break;
					}

				if (m_written == m_sourceFile->size())
					break;
			}
			else
				if (m_skipAllIfNotCopy || tryAgain)
					break;
				else
					askForSkipIfNotCopy(
							tr("Failed to copy..."),
							tr("Failed to create file \"%1\" (%2). Skip it?").
								arg(destination->location(entry->fileName())).
								arg(m_lastError),
							tryAgain = false,
							aborted);
		else
			if (m_skipAllIfNotCopy || tryAgain)
				break;
			else
				askForSkipIfNotCopy(
						tr("Failed to copy..."),
						tr("Failed to open file \"%1\" (%2). Skip it?").
							arg(source->location(entry->fileName())).
							arg(m_lastError),
						tryAgain = false,
						aborted);
	while (tryAgain && !aborted);
}

FILE_SYSTEM_NS_END
