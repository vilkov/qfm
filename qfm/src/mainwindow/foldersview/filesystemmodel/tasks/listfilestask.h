#ifndef FILEINFOGATHERERTASK_H_
#define FILEINFOGATHERERTASK_H_

#include "filestask.h"


class ListFilesTask : public FilesTask
{
public:
	ListFilesTask(const QString &directory, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* FILEINFOGATHERERTASK_H_ */
