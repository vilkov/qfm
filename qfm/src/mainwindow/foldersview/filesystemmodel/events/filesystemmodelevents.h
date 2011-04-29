#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "filesystemmodelevent.h"
#include "../items/filesystemtree.h"
#include "../items/filesystementry.h"
#include "../../../../tools/taskspool/task.h"


struct FileSystemModelEvents
{
	class BaseTask : public TasksPool::Task
	{
	public:
		struct Params : public MemoryManagerTag
		{
			struct Listener
			{
				Listener() :
					object(0),
					fileSystemTree(0)
				{}
				Listener(QObject *object, FileSystemTree *fileSystemTree) :
					object(object),
					fileSystemTree(fileSystemTree)
				{}

				QObject *object;
				FileSystemTree *fileSystemTree;
			};
			struct Snapshot
			{
				Snapshot() :
					object(0),
					fileSystemTree(0),
					entry(0)
				{}
				Snapshot(QObject *object, FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
					object(object),
					fileSystemTree(fileSystemTree),
					entry(entry)
				{}

				QObject *object;
				FileSystemTree *fileSystemTree;
				FileSystemEntry *entry;
			};
		};
	};


	class BaseEvent : public FileSystemModelEvent
	{
	public:
		struct Params : public FileSystemModelEvent::Params
		{
			struct Snapshot
			{
				Snapshot() :
					fileSystemTree(0),
					entry(0)
				{}
				Snapshot(FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
					fileSystemTree(fileSystemTree),
					entry(entry)
				{}
				Snapshot(const Params::Snapshot &listener) :
					fileSystemTree(listener.fileSystemTree),
					entry(listener.entry)
				{}

				Snapshot &operator=(const BaseTask::Params::Snapshot &other)
				{
					fileSystemTree = other.fileSystemTree;
					entry = other.entry;
					return *this;
				}

				FileSystemTree *fileSystemTree;
				FileSystemEntry *entry;
			};
		};

	protected:
		BaseEvent(Type type) :
			FileSystemModelEvent(type)
		{}
	};


private:
	template <typename T, int EType>
	class TemplateEvent : public BaseEvent
	{
	public:
		TemplateEvent() :
			BaseEvent(static_cast<Type>(EType))
		{}

		virtual const FileSystemModelEvent::Params *parameters() const { return &m_params; }

	public:
		typedef T Params;

		const Params &params() const { return m_params; }
		Params &params() { return m_params; }

	private:
		Params m_params;
	};


public:
	/********** ListFiles **********/
	struct ListFilesParams : public BaseEvent::Params
	{
		FileSystemTree *fileSystemTree;
		QList<FileSystemInfo> updates;
		bool isLastEvent;
	};
	typedef TemplateEvent<ListFilesParams, BaseEvent::ListFiles> ListFilesEvent;


	/********** UpdateFiles **********/
	struct UpdateFilesParams : public BaseEvent::Params
	{
		FileSystemTree *fileSystemTree;
		bool isLastEvent;
		ChangesList updates;
	};
	typedef TemplateEvent<UpdateFilesParams, BaseEvent::UpdateFiles> UpdateFilesEvent;


	/********** ScanFiles **********/
	struct ScanFilesParams : public BaseEvent::Params
	{
		Snapshot snapshot;
		FileSystemTree *subtree;
		quint64 size;
	};
	typedef TemplateEvent<ScanFilesParams, BaseEvent::ScanFilesForRemove> ScanFilesForRemoveEvent;
	typedef TemplateEvent<ScanFilesParams, BaseEvent::ScanFilesForSize> ScanFilesForSizeEvent;


	/********** ScanFilesWithDest **********/
	struct ScanFilesWithDestParams : public ScanFilesParams
	{
		BaseTask::Params::Listener destination;
	};
	typedef TemplateEvent<ScanFilesWithDestParams, BaseEvent::ScanFilesForCopy> ScanFilesForCopyEvent;
	typedef TemplateEvent<ScanFilesWithDestParams, BaseEvent::ScanFilesForMove> ScanFilesForMoveEvent;


	/********** Perform **********/
	struct PerformParams : public BaseEvent::Params
	{
		Snapshot snapshot;
	};


	/********** PerformRemoveFiles **********/
	struct PerformRemoveFilesParams: public PerformParams
	{
		bool removeParentEntry;
	};
	typedef TemplateEvent<PerformRemoveFilesParams, BaseEvent::RemoveFilesCompleted> RemoveFilesCompletedEvent;
	typedef TemplateEvent<PerformRemoveFilesParams, BaseEvent::RemoveFilesCanceled> RemoveFilesCanceledEvent;


	/********** PerformWithDest **********/
	struct PerformWithDestParams : public PerformParams
	{
		BaseTask::Params::Listener destination;
	};


	/********** PerformWithDestCopyFiles **********/
	struct PerformWithDestCopyFilesParams : public PerformWithDestParams
	{
		bool removeSource;
	};
	typedef TemplateEvent<PerformWithDestCopyFilesParams, BaseEvent::CopyFilesCompleted> CopyFilesCompletedEvent;
	typedef TemplateEvent<PerformWithDestCopyFilesParams, BaseEvent::CopyFilesCanceled> CopyFilesCanceledEvent;


	/********** PerformWithDestCopyTreeFiles **********/
	struct PerformWithDestCopyTreeFilesParams : public PerformWithDestCopyFilesParams
	{
		FileSystemTree *subtree;
	};
	typedef TemplateEvent<PerformWithDestCopyTreeFilesParams, BaseEvent::CopyFilesCompleted> CopyTreeFilesCompletedEvent;
	typedef TemplateEvent<PerformWithDestCopyTreeFilesParams, BaseEvent::CopyFilesCanceled> CopyTreeFilesCanceledEvent;


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
		Snapshot snapshot;
		quint64 progress;
		quint64 timeElapsed;
	};
	typedef TemplateEvent<UpdatePerformProgressParams, BaseEvent::UpdatePerformProgress> UpdatePerformProgressEvent;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
