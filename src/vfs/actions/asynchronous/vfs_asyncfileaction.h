#ifndef VFS_ASYNCFILEACTION_H_
#define VFS_ASYNCFILEACTION_H_

#include "../vfs_fileaction.h"
#include "../../tasks/vfs_basetask.h"


VFS_NS_BEGIN
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

VFS_NS_END

#endif /* VFS_ASYNCFILEACTION_H_ */
