#ifndef FILEINFOGATHERERTASK_H_
#define FILEINFOGATHERERTASK_H_

#include "filestask.h"


class ListFilesTask : public FilesTask
{
public:
	struct Params : public ControlableTask::Params
	{
		QObject *object;
		FileSystemTree *fileSystemTree;
		ChangesList list;
	};
	struct EventParams : public FilesTask::EventParams
	{
		FileSystemTree *fileSystemTree;
		bool isLastEvent;
		QList<FileSystemInfo> updates;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ListFilesTask(Event::EventType eventType, Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(FilesTask::parameters()); }

private:
	Event::EventType m_eventType;
};

#endif /* FILEINFOGATHERERTASK_H_ */
