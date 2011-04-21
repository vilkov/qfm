#ifndef COMMONTASKSEVENTS_H_
#define COMMONTASKSEVENTS_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "../events/filesystemmodelevents.h"


namespace CommonTasksEvents {

	struct QuestionAnswerParams : public FileSystemModelEvent::EventParams
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

}

#endif /* COMMONTASKSEVENTS_H_ */
