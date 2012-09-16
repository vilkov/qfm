#ifndef ARC_READARCHIVETASK_H_
#define ARC_READARCHIVETASK_H_

#include "arc_filesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class ReadArchiveTask : public FilesBaseTask
{
public:
	ReadArchiveTask(const IFileContainer *container, TasksNode *receiver);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	const IFileContainer *m_container;
};

ARC_PLUGIN_NS_END

#endif /* ARC_READARCHIVETASK_H_ */
