#ifndef FILESYSTEMTASKDIALOG_H_
#define FILESYSTEMTASKDIALOG_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "../filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN

class QuestionEvent: public BaseTask::Event
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

		bool waitFor(const volatile BaseTask::Flags &aborted)
		{
			QMutexLocker locker(&m_mutex);

			while (!aborted)
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
	QuestionEvent(const QString &title, const QString &question, qint32 buttons, Result *result) :
		Event(NULL, Question),
		m_title(title),
		m_question(question),
		m_buttons(buttons),
		m_result(result)
	{}

	const QString &title() const { return m_title; }
	const QString &question() const { return m_question; }
	qint32 buttons() const { return m_buttons; }
	Result *result() const { return m_result; }

private:
	QString m_title;
	QString m_question;
	qint32 m_buttons;
	Result *m_result;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKDIALOG_H_ */
