#include "arcperformcopytask.h"
#include "../nodes/items/arcnodedirentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


ARC_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(const QString &fileName, const ArcNodeItem *item, PScopedPointer<ICopyControl> &control, bool move, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName),
	m_item(item),
	m_control(control.take()),
	m_move(move),
	m_overwriteAll(false),
	m_skipAllIfNotCopy(false),
	m_progress(receiver)
{}

IFile::value_type *PerformCopyTask::buffer() const
{
	return const_cast<IFile::value_type *>(m_buffer);
}

IFile::size_type PerformCopyTask::bufferSize() const
{
	return FileReadWriteGranularity;
}

void PerformCopyTask::progressInit(const ArcNodeItem *item)
{
	m_progress.init(item);
}

void PerformCopyTask::progressUpdate(quint64 progressIncrement)
{
	m_progress.update(progressIncrement);
}

void PerformCopyTask::progresscomplete()
{
	m_progress.complete();
}

bool PerformCopyTask::overwriteAll() const
{
	return m_overwriteAll;
}

bool PerformCopyTask::skipAllIfNotCopy() const
{
	return m_skipAllIfNotCopy;
}

void PerformCopyTask::askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::No |
						QMessageBox::NoToAll |
						QMessageBox::Cancel,
						aborted);

	switch (answer)
	{
		case QMessageBox::Yes:
			tryAgain = true;
			break;

		case QMessageBox::YesToAll:
			m_overwriteAll = tryAgain = true;
			break;

		case QMessageBox::No:
			tryAgain = false;
			break;

		case QMessageBox::NoToAll:
			m_overwriteAll = tryAgain = false;
			break;

		case QMessageBox::Cancel:
			cancel();
			break;
	}
}

void PerformCopyTask::askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::Retry |
						QMessageBox::Cancel,
						aborted);

	if (answer == QMessageBox::YesToAll)
		m_skipAllIfNotCopy = true;
	else
		if (answer == QMessageBox::Retry)
			tryAgain = true;
		else
			if (answer == QMessageBox::Cancel)
				cancel();
}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	Archive::State *state;
	PScopedPointer<Event> event(new Event(this, m_move));

	if (const Archive *archive = Archive::archive(m_fileName, &state))
	{
		archive->extract(state, m_item, m_control.data(), this, aborted);
		archive->endRead(state);
	}

	event->item = m_item;
	event->move = m_move;
	event->canceled = aborted;
	postEvent(event.take());
}

ARC_PLUGIN_NS_END
