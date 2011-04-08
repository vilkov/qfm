#ifndef POPULATEFILESTASK_H_
#define POPULATEFILESTASK_H_

#include "filestask.h"


class PopulateFilesTask : public FilesTask
{
public:
	PopulateFilesTask(FileSystemTree *tree, const QString &directory, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);

private:
	void populate(FileSystemTree *tree, const volatile bool &stopedFlag);

private:
	FileSystemTree *m_subtree;
};

#endif /* POPULATEFILESTASK_H_ */
