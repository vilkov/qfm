#ifndef FILEINFOGATHERERTASK_H_
#define FILEINFOGATHERERTASK_H_

#include "filestask.h"


class ListFilesTask : public FilesTask
{
public:
	ListFilesTask(FileSystemTree *tree, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* FILEINFOGATHERERTASK_H_ */
