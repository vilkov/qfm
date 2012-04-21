#ifndef FILESYSTEMFOLDERCOPYACTION_H_
#define FILESYSTEMFOLDERCOPYACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderCopyAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCopyAction)

public:
	FolderCopyAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERCOPYACTION_H_ */
