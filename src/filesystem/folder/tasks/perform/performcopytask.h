#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../taskprogress.h"
#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyTask : public PerformTask<DestControlableTask>
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 1 * 1024 * 1024 };
	typedef PerformTask<DestControlableTask>        parent_class;
	typedef ModelEvents::CopyFilesCompletedEvent    CompletedEvent;
	typedef ModelEvents::QuestionAnswerEvent        QuestionAnswerEvent;
	typedef ModelEvents::UpdatePerformProgressEvent UpdateProgressEvent;

public:
	struct Params : public parent_class::Params
	{
		Params(QObject *receiver, QScopedPointer<FileSystemList> &entries, IFileControl *dest, bool move) :
			receiver(receiver),
			entries(entries.take()),
			removeSource(move)
		{
			destination = dest;
		}
		~Params()
		{
			delete destination;
		}

		QObject *receiver;
		QScopedPointer<FileSystemList> entries;
		bool removeSource;
	};

public:
	PerformCopyTask(QObject *receiver, QScopedPointer<FileSystemList> &entries, IFileControl *destination, bool move);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

	void copyEntry(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void copyFile(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &stopedFlag);
//	void copyFile(IFileInfo *destination, FolderNodeEntry *entry, const volatile bool &stopedFlag);

protected:
//	struct Messages
//	{
//		Q_DECLARE_TR_FUNCTIONS(PerformCopyTask::Messages)
//
//	public:
//		static QString failedToCopy(IFileInfo *source, IFileInfo *destination, const QString &error);
//		static QString alreadyExists(IFileInfo *source, IFileInfo *destination);
//	};

protected:
	bool m_tryAgain;
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
//	friend class CopyProgressRoutine;

private:
	IFile *m_destFile;
	IFileControl *m_destEntry;
	IFile::size_type m_readed;
	IFile::size_type m_written;
	IFile::value_type m_buffer[FileReadWriteGranularity];
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYTASK_H_ */
