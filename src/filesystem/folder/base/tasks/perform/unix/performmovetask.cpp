#include "../performmovetask.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>


FILE_SYSTEM_NS_BEGIN

PerformMoveTask::PerformMoveTask(TasksNode *receiver, const Snapshot &snapshot, IFileContainer::Holder &destination) :
	PerformCopyTask(receiver, snapshot, destination, true)
{}

void PerformMoveTask::copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	m_source = source->location(entry->fileName()).toUtf8();
	m_dest = destination->location(entry->fileName()).toUtf8();

	do
		if (::link(m_source, m_dest) == 0)
		{
			m_progress.update(entry->fileSize());
			break;
		}
		else
			if (errno == EEXIST)
			{
				if (::unlink(m_dest) == 0)
					tryAgain = true;
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to remove file \"%1\" (%2). Skip it?").
									arg(destination->location(entry->fileName())).
									arg(QString::fromUtf8(::strerror(errno))),
								tryAgain = false,
								aborted);
			}
			else
			{
				PerformCopyTask::copyFile(destination, source, entry, tryAgain, aborted);
				break;
			}
	while (tryAgain && !aborted);
}

FILE_SYSTEM_NS_END
