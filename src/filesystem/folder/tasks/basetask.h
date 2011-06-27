#ifndef BASETASK_H_
#define BASETASK_H_

#include <QtCore/QMutex>
#include <QtCore/QStringList>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "items/filesystemlist.h"
#include "items/filesystementry.h"
#include "../info/filesystemfoldernodeinfo.h"
#include "../events/filesystemmodelevent.h"
#include "../containers/filesystemupdateslist.h"
#include "../containers/filesystemfoldernodevalues.h"
#include "../../interfaces/filesysteminode.h"
#include "../../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN

class BaseTask : public TasksPool::Task
{
public:
	struct Entry
	{
		Entry()
		{}
		Entry(Values::size_type index, const QString &fileName) :
			index(index),
			fileName(fileName)
		{}

		Values::size_type index;
		QString fileName;
	};
	typedef QList<Entry> EntryList;


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
			fileName(),
			progress(),
			timeElapsed()
		{}

		QString fileName;
		quint64 progress;
		quint64 timeElapsed;
	};
};

FILE_SYSTEM_NS_END

#endif /* BASETASK_H_ */
