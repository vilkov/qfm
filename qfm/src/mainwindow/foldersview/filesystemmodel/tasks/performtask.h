#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "controlabletask.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"


class PerformTask : public ControlableTask
{
public:
	struct Params : public ControlableTask::Params
	{
		QObject *receiver;
		FileSystemTree *fileSystemTree;
	};
	struct EventParams : public FileSystemModelEvent::Params
	{
		FileSystemTree *fileSystemTree;
	};

	struct QuestionAnswerParams : public FileSystemModelEvent::Params
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

			bool waitFor(const volatile bool &stopedFlag)
			{
				QMutexLocker locker(&m_mutex);

				while (!stopedFlag)
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
};

#endif /* PERFORMTASK_H_ */
