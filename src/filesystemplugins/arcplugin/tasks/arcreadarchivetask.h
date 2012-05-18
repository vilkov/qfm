#ifndef ARCREADARCHIVETASK_H_
#define ARCREADARCHIVETASK_H_

#include "arcfilesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class ReadArchiveTask : public FilesBaseTask
{
public:
	ReadArchiveTask(IFileContainer::Holder &container, TasksNode *receiver);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	IFileContainer::Holder m_container;
};

ARC_PLUGIN_NS_END

#endif /* ARCREADARCHIVETASK_H_ */
