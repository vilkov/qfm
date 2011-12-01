#include "performremovetask.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, PScopedPointer<InfoListItem> &entries) :
	PerformTask(receiver),
	m_progress(receiver),
	m_entries(entries.take())
{}

void PerformRemoveTask::run(const volatile bool &aborted)
{
	bool tryAgain;
	InfoItem *entry;

	for (InfoListItem::size_type i = 0;
			i < m_entries->size() &&
			!isCanceled() &&
			!isReceiverDead() &&
			!aborted;
			++i)
	{
		if ((entry = m_entries->at(i))->isDir())
		{
			m_progress.init(entry->fileName());
			removeEntry(entry, tryAgain = false, aborted);
			m_progress.complete();
		}
		else
		{
			m_progress.clear();
			removeEntry(entry, tryAgain = false, aborted);
		}
	}

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(Event::RemoveFiles, isCanceled(), m_entries));
		QCoreApplication::postEvent(receiver(), event.take());
	}
}

void PerformRemoveTask::removeEntry(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted)
{
	entry->refresh();

	if (entry->exists())
		if (entry->isDir())
		{
			InfoItem *localEntry;

			for (InfoListItem::size_type i = 0, size = static_cast<InfoListItem*>(entry)->size(); i < size; ++i)
			{
				removeEntry(localEntry = static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);

				if (!localEntry->shouldRemove())
					entry->setShouldRemove(false);
			}

			if (entry->shouldRemove())
				do
					removeDir(entry, tryAgain = false, aborted);
				while (tryAgain && !isCanceled() && !isReceiverDead() && !aborted);
		}
		else
			do
				removeFile(entry, tryAgain = false, aborted);
			while (tryAgain && !isCanceled() && !isReceiverDead() && !aborted);
}

void PerformRemoveTask::removeDir(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted)
{
	QDir dir = entry->absolutePath();

	if (!dir.rmdir(entry->fileName()))
		if (m_skipAllIfNotRemove)
			entry->setShouldRemove(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("Directory \"%1\". Skip it?").arg(entry->absoluteFilePath()),
								QMessageBox::Yes |
								QMessageBox::YesToAll |
								QMessageBox::Retry |
								QMessageBox::Cancel,
								aborted);

			if (answer == QMessageBox::YesToAll)
			{
				m_skipAllIfNotRemove = true;
				entry->setShouldRemove(false);
			}
			else
				if (answer == QMessageBox::Retry)
					tryAgain = true;
				else
				{
					if (answer == QMessageBox::Cancel)
						cancel();

					entry->setShouldRemove(false);
				}
		}
}

void PerformRemoveTask::removeFile(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted)
{
	if (!doRemoveFile(entry->absoluteFilePath(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setShouldRemove(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("File \"%1\" (%2). Skip it?").arg(entry->absoluteFilePath()).arg(m_error),
								QMessageBox::Yes |
								QMessageBox::YesToAll |
								QMessageBox::Retry |
								QMessageBox::Cancel,
								aborted);

			if (answer == QMessageBox::YesToAll)
			{
				m_skipAllIfNotRemove = true;
				entry->setShouldRemove(false);
			}
			else
				if (answer == QMessageBox::Retry)
					tryAgain = true;
				else
				{
					if (answer == QMessageBox::Cancel)
						cancel();

					entry->setShouldRemove(false);
				}
		}
}

bool PerformRemoveTask::doRemoveFile(const QString &filePath, QString &error)
{
#ifdef Q_OS_WIN32
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif

	QFile file(filePath);

	if (file.open(QFile::ReadOnly))
	{
		m_progress.update(file.size());
		file.close();

		if (file.remove())
			return true;
		else
			error = file.errorString();
	}
	else
		error = file.errorString();

	return false;
}

FILE_SYSTEM_NS_END
