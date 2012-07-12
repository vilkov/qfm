#ifndef DEFAULTSEARCHTASK_H_
#define DEFAULTSEARCHTASK_H_

#include "../../../tasks/defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchTask : public FilesBaseTask
{
public:
	SearchTask(const IFileContainer *container, TasksNode *receiver);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	const IFileContainer *m_container;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHTASK_H_ */
