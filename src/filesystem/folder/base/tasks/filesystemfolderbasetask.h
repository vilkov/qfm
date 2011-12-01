#ifndef FILESYSTEMFOLDERBASETASK_H_
#define FILESYSTEMFOLDERBASETASK_H_

#include "../../../tasks/filesystembasetask.h"
#include "../../../containers/filesysteminfolistitem.h"
#include "../../../containers/filesysteminfoentryitem.h"
#include "../../../../tools/pointers/pscopedpointer.h"

FILE_SYSTEM_NS_BEGIN

class FolderBaseTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			UpdateFiles = User,
			ScanFilesForSize = User + 1,
			ScanFilesForRemove = User + 2,
			ScanFilesForCopy = User + 3,
			RemoveFiles = User + 4,
			CopyFiles = User + 5,
			UpdateProgress = User + 6,
			CompletedProgress = User + 7,
		};

	protected:
		Event(Type type) :
			BaseTask::Event(static_cast<BaseTask::Event::Type>(type))
		{}
	};


	class UpdateProgressEvent : public Event
	{
	public:
		UpdateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed) :
			Event(UpdateProgress),
			fileName(fileName),
			progress(progress),
			timeElapsed(timeElapsed)
		{}

		QString fileName;
		quint64 progress;
		quint64 timeElapsed;
	};


	class CompletedProgressEvent : public Event
	{
	public:
		CompletedProgressEvent(const QString &fileName, quint64 timeElapsed) :
			Event(CompletedProgress),
			fileName(fileName),
			timeElapsed(timeElapsed)
		{}

		QString fileName;
		quint64 timeElapsed;
	};

public:
	FolderBaseTask(TasksNode *receiver) :
		BaseTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERBASETASK_H_ */
