#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "../controlabletask.h"


class PerformTask : public ControlableTask
{
public:
	struct Params : public ControlableTask::Params
	{
		Snapshot source;
	};
	struct EventParams : public ControlableTask::EventParams
	{
		Snapshot snapshot;
	};

	struct QuestionAnswerParams : public ControlableTask::EventParams
	{
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
			qint32 answer() { return m_answer; }

		private:
			qint32 m_answer;
			QMutex m_mutex;
			QWaitCondition m_condition;
		};

		QString title;
		QString question;
		QMessageBox::StandardButtons buttons;
		Result *result;
	};
	typedef FileSystemModelEventTemplate<QuestionAnswerParams> QuestionAnswerEvent;

public:
	PerformTask(Params *params, QObject *controller1);
	PerformTask(Params *params, QObject *controller1, QObject *controller2);
	PerformTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3);

protected:
	inline Params *parameters() const { return static_cast<Params*>(ControlableTask::parameters()); }

protected:
	volatile bool m_canceled;
};

#endif /* PERFORMTASK_H_ */
