#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <QtGui/QMessageBox>
#include "controlabletask.h"
#include "scanfilestask.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"


/********************************************************************************************************/
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


/********************************************************************************************************/
class PerformRemoveTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	struct Params : public PerformTask::Params
	{
		Params(QObject *rcv, const ScanFilesForRemoveTask::EventParams &params) :
			entry(params.entry),
			subtree(params.subtree)
		{
			receiver = rcv;
			fileSystemTree = params.fileSystemTree;
		}

		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	struct EventParams : public PerformTask::EventParams
	{
		FileSystemEntry *entry;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	PerformRemoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

private:
	void remove(FileSystemTree *tree, const volatile bool &stopedFlag);
	void removeEntry(FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);

private:
	bool m_skipAllIfNotRemove;
	bool m_skipAllIfNotExists;
	volatile bool m_canceled;
};

#endif /* PERFORMTASK_H_ */
