#ifndef FILEINFOGATHERERTASK_H_
#define FILEINFOGATHERERTASK_H_

#include "filestask.h"


class ListFilesTask : public FilesTask
{
public:
	struct EventParams : public FilesTask::EventParams
	{
		bool isLastEvent;
		QList<FileSystemInfo> updates;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ListFilesTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* FILEINFOGATHERERTASK_H_ */
