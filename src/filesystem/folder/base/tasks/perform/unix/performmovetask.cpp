#include "../performmovetask.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>


FILE_SYSTEM_NS_BEGIN

PerformMoveTask::PerformMoveTask(TasksNode *receiver, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control) :
	PerformCopyTask(receiver, entries, control, true)
{}

void PerformMoveTask::copyFile(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	m_source = entry->absoluteFilePath().toUtf8();
	m_dest = destination->location().append(QChar('/')).append(entry->fileName()).toUtf8();

	do
		if (link(m_source, m_dest) == 0)
			break;
		else
			if (errno == EEXIST)
			{
				if (unlink(m_dest) == 0)
					tryAgain = true;
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to create file \"%1\" (%2). Skip it?").
									arg(destination->location(entry->fileName())).
									arg(QString::fromUtf8(strerror(errno))),
								tryAgain = false,
								aborted);
			}
			else
				if (m_skipAllIfNotCopy)
					break;
				else
					askForSkipIfNotCopy(
							tr("Failed to copy..."),
							tr("Failed to create file \"%1\" (%2). Skip it?").
								arg(destination->location(entry->fileName())).
								arg(QString::fromUtf8(strerror(errno))),
							tryAgain = false,
							aborted);
	while (tryAgain && !aborted);
}

FILE_SYSTEM_NS_END
