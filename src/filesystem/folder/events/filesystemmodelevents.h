#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "filesystemmodelevent.h"
#include "../filesystemfoldernode.h"
#include "../items/filesystemfoldernodeentry.h"
#include "../../filesystemnode.h"
#include "../../filesystem_ns.h"
#include "../../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN

struct ModelEvents
{
	class FolderNode : public FileSystem::FolderNode
	{
		Q_DISABLE_COPY(FolderNode)

	public:
		FolderNode(const Info &info, Node *parent = 0) :
			FileSystem::FolderNode(info, parent)
		{}

		void add(const Info &info) { items().add(new FolderNodeEntry(info)); }
		void setSubnode(FileSystem::FolderNode *subnode)
		{
			FileSystem::FolderNode::Values::size_type index = items().indexOf(subnode->fileName());

			if (index != FileSystem::FolderNode::Values::InvalidIndex)
				items()[index].node = subnode;
		}
	};

	class BaseTask : public TasksPool::Task
	{
	public:
		struct Params : public MemoryManagerTag
		{
			struct Listener
			{
				Listener() :
					node(0)
				{}
				Listener(Node *node) :
					node(node)
				{}

				Node *node;
			};
			struct Snapshot
			{
				Snapshot() :
					node(0),
					entry(0)
				{}
				Snapshot(Node *node, FolderNodeEntry *entry) :
					node(node),
					entry(entry)
				{}

				Node *node;
				FolderNodeEntry *entry;
			};
		};
	};


	class BaseEvent : public ModelEvent
	{
	public:
		struct Params : public ModelEvent::Params
		{
			struct Snapshot
			{
				Snapshot() :
					entry(0)
				{}
				Snapshot(FolderNodeEntry *entry) :
					entry(entry)
				{}
				Snapshot(const Params::Snapshot &listener) :
					entry(listener.entry)
				{}

				Snapshot &operator=(const BaseTask::Params::Snapshot &other)
				{
					entry = other.entry;
					return *this;
				}

				FolderNodeEntry *entry;
			};
		};

	protected:
		BaseEvent(Type type) :
			ModelEvent(type)
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

		virtual const ModelEvent::Params *parameters() const { return &m_params; }

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
		QList<Info> updates;
		bool isLastEvent;
	};
	typedef TemplateEvent<ListFilesParams, BaseEvent::ListFiles> ListFilesEvent;


	/********** UpdateFiles **********/
	struct UpdateFilesParams : public BaseEvent::Params
	{
		bool isLastEvent;
		ChangesList updates;
	};
	typedef TemplateEvent<UpdateFilesParams, BaseEvent::UpdateFiles> UpdateFilesEvent;


	/********** ScanFiles **********/
	struct ScanFilesParams : public BaseEvent::Params
	{
		Snapshot snapshot;
		Node *subnode;
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
		Node *subnode;
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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMMODELEVENTS_H_ */
