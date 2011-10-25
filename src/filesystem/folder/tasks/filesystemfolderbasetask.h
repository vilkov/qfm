#ifndef FILESYSTEMFOLDERBASETASK_H_
#define FILESYSTEMFOLDERBASETASK_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "../base/events/filesystemmodelevent.h"
#include "../../tasks/filesystembasetask.h"
#include "../../containers/filesysteminfolistitem.h"
#include "../../containers/filesysteminfoentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class FolderBaseTask : public BaseTask
{
public:
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
	FolderBaseTask(QObject *receiver) :
		BaseTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERBASETASK_H_ */
