#include "filesystembasetask.h"
#include "filesystemtasksnode.h"
#include "tools/filesystemtaskdialog.h"
#include "../../application.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

BaseTask::BaseTask(TasksNode *receiver) :
	Task(),
	m_receiver(receiver),
	m_canceled(1, flags())
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

void BaseTask::postEvent(Event *event) const
{
	Application::postEvent(m_receiver, event);
}

qint32 BaseTask::askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const
{
	QuestionEvent::Result result;
	postEvent(new QuestionEvent(title, question, buttons, &result));

	if (result.waitFor(aborted))
		return result.answer();
	else
		return 0;
}

FILE_SYSTEM_NS_END
