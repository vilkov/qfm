#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../taskprogress.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

	class Event : public PerformTask::Event
	{
	public:
		Event(bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move) :
			PerformTask::Event(CopyFiles, canceled, entries),
			destination(destination.take()),
			move(move)
		{}

		PScopedPointer<IFileControl> destination;
		bool move;
	};

public:
	PerformCopyTask(QObject *receiver, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move);

	virtual void run(const volatile bool &aborted);

protected:
	void copyEntry(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void copyFile(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &aborted);

private:
	PScopedPointer<FileSystemList> m_entries;
	PScopedPointer<IFileControl> m_destination;
	bool m_move;
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
	PScopedPointer<IFile> m_destFile;
	PScopedPointer<IFile> m_sourceFile;
	PScopedPointer<IFileControl> m_destEntry;
	IFile::size_type m_readed;
	IFile::size_type m_written;
	IFile::value_type m_buffer[FileReadWriteGranularity];
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYTASK_H_ */
