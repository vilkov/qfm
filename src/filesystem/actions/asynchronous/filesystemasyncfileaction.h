#ifndef FILESYSTEMASYNCFILEACTION_H_
#define FILESYSTEMASYNCFILEACTION_H_

#include <QtCore/QList>
#include "../filesystemfileaction.h"
#include "../../tasks/filesystembasetask.h"
#include "../../tasks/items/filesystemtasknodeitem.h"
#include "../../interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN
class PerformActionTask;


class AsyncFileAction : public FileAction
{
public:
	typedef BaseTask::Flags                                   Flags;
	typedef QPair<const TaskNodeItem *, const IFileControl *> FileItem;
	typedef QList<FileItem>                                   FilesList;

public:
	virtual bool isAsynchronous() const;
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMASYNCFILEACTION_H_ */
