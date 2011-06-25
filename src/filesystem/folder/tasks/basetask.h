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
#include "../../interfaces/filesysteminode.h"
#include "../../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN

class BaseTask : public TasksPool::Task
{
public:
	struct Params : public MemoryManagerTag
	{
		struct Snapshot
		{
			Snapshot(QObject *listener, const Info &info, const QStringList &entries) :
				info(info),
				entries(entries),
				listener(listener)
			{}

			Info info;
			QStringList entries;
			QObject *listener;
		};
	};
};


class BaseEvent : public ModelEvent
{
public:
	struct Params : public ModelEvent::Params
	{};

protected:
	BaseEvent(Type type) :
		ModelEvent(type)
	{}
};


struct ModelEvents
{
private:
	template <typename T, int EType>
	class TemplateEvent : public BaseEvent
	{
	public:
		typedef T Params;

	public:
		TemplateEvent() :
			BaseEvent(static_cast<Type>(EType))
		{}

		virtual const ModelEvent::Params *parameters() const { return &m_params; }

		const Params &params() const { return m_params; }
		Params &params() { return m_params; }

	private:
		Params m_params;
	};


public:
	/********** ListFiles **********/
	struct ListFilesParams : public BaseEvent::Params
	{
		QList<Info> updates;
		bool isLastEvent;
	};
	typedef TemplateEvent<ListFilesParams, BaseEvent::ListFiles> ListFilesEvent;


	/********** UpdateFiles **********/
	struct UpdateFilesParams : public BaseEvent::Params
	{
		UpdateFilesParams(bool isLastEvent, const UpdatesList &updates) :
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		UpdatesList updates;
	};
	class UpdateFilesEvent : public BaseEvent
	{
	public:
		typedef UpdateFilesParams Params;

	public:
		UpdateFilesEvent(bool isLastEvent, const UpdatesList &updates) :
			BaseEvent(BaseEvent::UpdateFiles),
			m_params(isLastEvent, updates)
		{}

		virtual const ModelEvent::Params *parameters() const { return &m_params; }

		const Params &params() const { return m_params; }
		Params &params() { return m_params; }

	private:
		Params m_params;
	};


	/********** ScanFiles **********/
	struct ScanFilesParams : public BaseEvent::Params
	{
		QScopedPointer<FileSystemList> subnode;
	};
	typedef TemplateEvent<ScanFilesParams, BaseEvent::ScanFilesForRemove> ScanFilesForRemoveEvent;
	typedef TemplateEvent<ScanFilesParams, BaseEvent::ScanFilesForSize> ScanFilesForSizeEvent;


	/********** ScanFilesWithDest **********/
	struct ScanFilesWithDestParams : public ScanFilesParams
	{
		IFileControl *destination;
		bool move;
	};
	typedef TemplateEvent<ScanFilesWithDestParams, BaseEvent::ScanFilesForCopy> ScanFilesForCopyEvent;


	/********** Perform **********/
	struct PerformParams : public BaseEvent::Params
	{};


	/********** PerformRemoveFiles **********/
	struct PerformRemoveFilesParams: public PerformParams
	{
		QScopedPointer<FileSystemList> subnode;
	};
	typedef TemplateEvent<PerformRemoveFilesParams, BaseEvent::RemoveFilesCompleted> RemoveFilesCompletedEvent;
	typedef TemplateEvent<PerformRemoveFilesParams, BaseEvent::RemoveFilesCanceled> RemoveFilesCanceledEvent;


	/********** PerformWithDest **********/
	struct PerformWithDestParams : public PerformParams
	{
		IFileControl *destination;
	};


	/********** PerformWithDestCopyFiles **********/
	struct PerformWithDestCopyFilesParams : public PerformWithDestParams
	{
		QScopedPointer<FileSystemList> entries;
		bool canceled;
		bool move;
	};
	typedef TemplateEvent<PerformWithDestCopyFilesParams, BaseEvent::CopyFilesCompleted> CopyFilesCompletedEvent;


	/********** PerformWithDestCopyTreeFiles **********/
	struct PerformWithDestCopyTreeFilesParams : public PerformWithDestCopyFilesParams
	{
		QScopedPointer<FileSystemList> subnode;
	};
	typedef TemplateEvent<PerformWithDestCopyTreeFilesParams, BaseEvent::CopyFilesCompleted> CopyTreeFilesCompletedEvent;


	/********** QuestionAnswer **********/
	struct QuestionAnswerParams : public BaseEvent::Params
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
			qint32 answer() const { return m_answer; }

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
	typedef TemplateEvent<QuestionAnswerParams, BaseEvent::QuestionAnswer> QuestionAnswerEvent;


	/********** UpdatePerformProgress **********/
	struct UpdatePerformProgressParams : public BaseEvent::Params
	{
		QString fileName;
		quint64 progress;
		quint64 timeElapsed;
	};
	typedef TemplateEvent<UpdatePerformProgressParams, BaseEvent::UpdatePerformProgress> UpdatePerformProgressEvent;
};

FILE_SYSTEM_NS_END

#endif /* BASETASK_H_ */
