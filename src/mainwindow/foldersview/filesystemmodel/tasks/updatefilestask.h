#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "filestask.h"
#include "../filesystemchangeslist.h"


class UpdateFilesTask : public FilesTask
{
public:
	UpdateFilesTask(FileSystemTree *tree, const ChangesList &list, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);

private:
    ChangesList::size_type indexOf(const QString &directoryPath, const ChangesList &list) const;

private:
    ChangesList m_list;
};

#endif /* UPDATEFILESTASK_H_ */
