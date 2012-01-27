#ifndef FILESYSTEMFOLDERCOPYACTION_H_
#define FILESYSTEMFOLDERCOPYACTION_H_

#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderCopyAction : public SyncFileAction
{
public:
	FolderCopyAction();

	virtual void process(const FilesList &files) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERCOPYACTION_H_ */
