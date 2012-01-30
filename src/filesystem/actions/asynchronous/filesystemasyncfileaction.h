#ifndef FILESYSTEMASYNCFILEACTION_H_
#define FILESYSTEMASYNCFILEACTION_H_

#include "../filesystemfileaction.h"
#include "../../tasks/filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN
class PerformActionTask;


class AsyncFileAction : public FileAction
{
public:
	typedef BaseTask::Flags Flags;

public:
	virtual bool isAsynchronous() const;

	virtual bool prepare(const FilesList &files) = 0;
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMASYNCFILEACTION_H_ */
