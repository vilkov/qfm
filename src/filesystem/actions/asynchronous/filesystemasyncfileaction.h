#ifndef FILESYSTEMASYNCFILEACTION_H_
#define FILESYSTEMASYNCFILEACTION_H_

#include "../filesystemfileaction.h"
#include "../../tasks/filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN
class TasksNode;
class PerformActionTask;


/**
 * This class and subclasses must be reentrant!
 */
class AsyncFileAction : public FileAction
{
public:
	typedef BaseTask::Flags Flags;

public:
	AsyncFileAction(const QIcon &icon, const QString &text);

	virtual bool isAsynchronous() const;

	virtual const QString &lockReason() const = 0;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMASYNCFILEACTION_H_ */
