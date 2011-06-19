#include "performcopytask.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(QObject *receiver, QScopedPointer<FileSystemList> &entries, INode *destination, bool move) :
	parent_class(new Params(receiver, entries, destination, move), receiver),
	m_tryAgain(false),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
//	m_progress.init();
//
//	if (parameters()->destination->exists())
//		copyFile(parameters()->destination, parameters()->source.entry, stopedFlag);
//	else
//		m_canceled = true;
//
//	if (!stopedFlag && !isControllerDead())
//	{
//		QScopedPointer<CompletedEvent> event(new CompletedEvent());
//		event->params().snapshot = parameters()->source;
//
//		if (m_canceled)
//		{
//			event->params().canceled = true;
//			event->params().removeSource = false;
//		}
//		else
//		{
//			event->params().canceled = false;
//			event->params().removeSource = parameters()->removeSource;
//		}
//
//		event->params().destination = parameters()->destination;
//		Application::postEvent(parameters()->source.node, event.take());
//	}
}

//void PerformCopyTask::copyFile(IFileInfo *destination, FolderNodeEntry *entry, const volatile bool &stopedFlag)
//{
//	bool ok;
//	IFile *sourceFile;
//
//	do
//		if (sourceFile = entry->open(IFile::ReadOnly, m_lastError))
//		{
//			ok = false;
//
//			if (destination->exists(entry))
//				if (m_overwriteAll)
//					ok = doCopyFile(destination, entry, sourceFile, stopedFlag);
//				else
//					askForOverwrite(
//							entry->lockReason(),
//							Messages::alreadyExists(entry, destination),
//							stopedFlag);
//			else
//				ok = doCopyFile(destination, entry, sourceFile, stopedFlag);
//
//			entry->close(sourceFile);
//
//			if (ok)
//				break;
//		}
//		else
//			if (m_skipAllIfNotCopy)
//				break;
//			else
//				askForSkipIfNotCopy(
//						entry->lockReason(),
//						Messages::failedToCopy(entry, destination, m_lastError),
//						stopedFlag);
//	while (m_tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
//}
//
//void PerformCopyTask::askForOverwrite(const QString &title, const QString &text, const volatile bool &stopedFlag)
//{
//	QuestionAnswerEvent::Params::Result result;
//	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
//	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll | QMessageBox::Cancel;
//	event->params().title = title;
//	event->params().result = &result;
//	event->params().question = text;
//
//	Application::postEvent(parameters()->source.node, event.take());
//
//	if (result.waitFor(stopedFlag, isControllerDead()))
//		switch (result.answer())
//		{
//			case QMessageBox::Yes:
//				m_tryAgain = true;
//				break;
//
//			case QMessageBox::YesToAll:
//				m_overwriteAll = m_tryAgain = true;
//				break;
//
//			case QMessageBox::No:
//				m_tryAgain = false;
//				break;
//
//			case QMessageBox::NoToAll:
//				m_overwriteAll = m_tryAgain = false;
//				break;
//
//			case QMessageBox::Cancel:
//				m_canceled = true;
//				break;
//		}
//}
//
//void PerformCopyTask::askForSkipIfNotCopy(const QString &title, const QString &text, const volatile bool &stopedFlag)
//{
//	QuestionAnswerEvent::Params::Result result;
//	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
//	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
//	event->params().title = title;
//	event->params().result = &result;
//	event->params().question = text;
//
//	Application::postEvent(parameters()->source.node, event.take());
//
//	if (result.waitFor(stopedFlag, isControllerDead()))
//		if (result.answer() == QMessageBox::YesToAll)
//			m_skipAllIfNotCopy = true;
//		else
//			if (result.answer() == QMessageBox::Retry)
//				m_tryAgain = true;
//			else
//				if (result.answer() == QMessageBox::Cancel)
//					m_canceled = true;
//}
//
//QString PerformCopyTask::Messages::failedToCopy(IFileInfo *source, IFileInfo *destination, const QString &error)
//{
//	return tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
//			arg(source->fileName()).
//			arg(error).
//			arg(source->absolutePath()).
//			arg(destination->absolutePath());
//}
//
//QString PerformCopyTask::Messages::alreadyExists(IFileInfo *source, IFileInfo *destination)
//{
//	return tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
//			arg(source->fileName()).
//			arg(source->absolutePath()).
//			arg(destination->absolutePath());
//}
//
//bool PerformCopyTask::doCopyFile(IFileInfo *destination, FolderNodeEntry *entry, IFile *sourceFile, const volatile bool &stopedFlag)
//{
//	bool res = false;
//
//	if ((m_destEntry = destination->create(entry, m_lastError)) == 0)
//		if (m_skipAllIfNotCopy)
//			res = true;
//		else
//			askForSkipIfNotCopy(
//					entry->lockReason(),
//					Messages::failedToCopy(entry, destination, m_lastError),
//					stopedFlag);
//	else
//	{
//		if (m_destFile = m_destEntry->open(IFile::WriteOnly, m_lastError))
//		{
//			m_written = 0;
//
//			while (m_readed = sourceFile->read(m_buffer, FileReadWriteGranularity))
//				if (m_destFile->write(m_buffer, m_readed) == m_readed)
//					m_written += m_readed;
//				else
//				{
//					m_lastError = m_destFile->lastError();
//
//					askForSkipIfNotCopy(
//							entry->lockReason(),
//							Messages::failedToCopy(entry, destination, m_lastError),
//							stopedFlag);
//
//					break;
//				}
//
//			m_destEntry->close(m_destFile);
//
//			if (m_written == sourceFile->size())
//				res = true;
//			else
//				if (!m_skipAllIfNotCopy)
//					askForSkipIfNotCopy(
//							entry->lockReason(),
//							Messages::failedToCopy(entry, destination, m_lastError),
//							stopedFlag);
//		}
//		else
//			res = true;
//
//		destination->close(m_destEntry);
//	}
//
//	return res;
//}

FILE_SYSTEM_NS_END
