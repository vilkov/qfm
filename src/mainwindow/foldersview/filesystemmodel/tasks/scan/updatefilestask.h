#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "filestask.h"
#include "../../filesystemchangeslist.h"


class UpdateFilesTask : public FilesTask
{
public:
	struct Params : public ControlableTask::Params
	{
		QObject *object;
		FileSystemTree *fileSystemTree;
		ChangesList list;
	};
	struct EventParams : public ControlableTask::EventParams
	{
		FileSystemTree *fileSystemTree;
		bool isLastEvent;
		ChangesList updates;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	UpdateFilesTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	Params *parameters() const { return static_cast<Params*>(FilesTask::parameters()); }

private:
    ChangesList::size_type indexOf(const QString &directoryPath, const ChangesList &list) const;
};

#endif /* UPDATEFILESTASK_H_ */