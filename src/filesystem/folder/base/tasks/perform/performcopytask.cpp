#include "performcopytask.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(TasksNode *receiver, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move) :
	PerformCopyBaseTask(receiver, entries, control),
	m_move(move)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	PerformCopyBaseTask::run(aborted);
	postEvent(new Event(this, isCanceled(), files(), control(), m_move));
}

void PerformCopyTask::copyFile(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (m_sourceFile = entry->open(IFileAccessor::ReadOnly, m_lastError))
			if (m_destEntry = destination->open(entry->fileName()))
				if (m_destFile = m_destEntry->open(IFileAccessor::ReadWrite | IFileAccessor::Create | IFileAccessor::Truncate, m_lastError))
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
										arg(m_destEntry->absoluteFilePath()).
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
									arg(m_destEntry->absoluteFilePath(entry->fileName())).
									arg(m_lastError),
								tryAgain = false,
								aborted);
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
							arg(entry->absoluteFilePath()).
							arg(m_lastError),
						tryAgain = false,
						aborted);
	while (tryAgain && !aborted);
}

FILE_SYSTEM_NS_END
