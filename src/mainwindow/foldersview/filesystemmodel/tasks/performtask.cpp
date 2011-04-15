#include "performtask.h"
#include "../../../../application.h"
#include <QtCore/QDir>
#include <QtCore/QMutex>


PerformTask::PerformTask(Params *params, QObject *controller1) :
	ControlableTask(params, controller1)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2) :
	ControlableTask(params, controller1, controller2)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	ControlableTask(params, controller1, controller2, controller3)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}


PerformRemoveTask::PerformRemoveTask(Params *params) :
	PerformTask(params, params->receiver)
{
	Q_ASSERT(params->entry);
	Q_ASSERT(params->subtree);
}

void PerformRemoveTask::run(const volatile bool &stopedFlag)
{
	QScopedPointer<FileSystemTree> subtree(parameters()->subtree);
	remove(subtree.data(), stopedFlag);
}

bool PerformRemoveTask::remove(FileSystemTree *tree, const volatile bool &stopedFlag)
{
	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag; ++i)
		if (static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().isDir())
		{
			QScopedPointer<FileSystemTree> subtree(static_cast<FileSystemTree*>(tree->subtree(tree->child(i))));
			remove(subtree.data(), stopedFlag);
		}
		else
		{
			QMessageBox m;
			QDir dir = static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().absoluteDir();

			if (dir.exists(static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().fileName()))
			{
				if (!dir.remove(static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().fileName()))
				{
					QMutex locker;
					QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
					event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
					event->params().question = tr("Failed to remove file \"%1\". Skip it?").arg(static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().absoluteFilePath());
					Application::postEvent(parameters()->receiver, event.data());
					event.take()->params().condition.wait(&locker);
				}
			}
			else
			{

			}
		}

	return true;
}
