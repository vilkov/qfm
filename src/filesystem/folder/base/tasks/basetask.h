#ifndef BASETASK_H_
#define BASETASK_H_

#include <QtCore/QMutex>
#include <QtCore/QStringList>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "../containers/filesystemupdateslist.h"
#include "../events/filesystemmodelevent.h"
#include "../../../list/filesystemlist.h"
#include "../../../list/filesystementry.h"
#include "../../../interfaces/filesysteminode.h"
#include "../../../../tools/taskspool/task.h"
#include "../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class BaseTask : public TasksPool::Task
{
public:
	typedef QStringList EntryList;


	class Event : public ModelEvent
	{
	public:
		Event(Type type, bool canceled) :
			ModelEvent(type),
			canceled(canceled)
		{}

		bool canceled;
	};

	class QuestionAnswerEvent: public ModelEvent
	{
	public:
		class Result
		{
		public:
			Result() :
				m_answer(0)
			{}

			void lock() { m_mutex.lock(); }
			void setAnswer(qint32 value) { m_answer = value; }
			void unlock() { m_mutex.unlock(); }

			bool waitFor(const volatile bool &stopedFlag, const volatile bool &isControllerDead)
			{
				QMutexLocker locker(&m_mutex);

				while (!stopedFlag && !isControllerDead)
				{
					m_condition.wait(&m_mutex, 1000);

					if (m_answer > 0)
						return true;
				}

				return false;
			}
			qint32 answer() const { return m_answer; }

		private:
			qint32 m_answer;
			QMutex m_mutex;
			QWaitCondition m_condition;
		};

	public:
		QuestionAnswerEvent(const QString &title, const QString &question, QMessageBox::StandardButtons buttons, Result *result) :
			ModelEvent(QuestionAnswer),
			m_title(title),
			m_question(question),
			m_buttons(buttons),
			m_result(result)
		{}

		const QString &title() const { return m_title; }
		const QString &question() const { return m_question; }
		QMessageBox::StandardButtons buttons() const { return m_buttons; }
		Result *result() const { return m_result; }

	private:
		QString m_title;
		QString m_question;
		QMessageBox::StandardButtons m_buttons;
		Result *m_result;
	};


	class UpdateProgressEvent : public ModelEvent
	{
	public:
		UpdateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed) :
			ModelEvent(UpdateProgress),
			fileName(fileName),
			progress(progress),
			timeElapsed(timeElapsed)
		{}

		QString fileName;
		quint64 progress;
		quint64 timeElapsed;
	};


	class CompletedProgressEvent : public ModelEvent
	{
	public:
		CompletedProgressEvent(const QString &fileName, quint64 timeElapsed) :
			ModelEvent(CompletedProgress),
			fileName(fileName),
			timeElapsed(timeElapsed)
		{}

		QString fileName;
		quint64 timeElapsed;
	};

public:
	BaseTask() :
		m_canceled(false)
	{}

	void cancel() { m_canceled = true; }

protected:
	const volatile bool isCanceled() const { return m_canceled; }

private:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* BASETASK_H_ */
