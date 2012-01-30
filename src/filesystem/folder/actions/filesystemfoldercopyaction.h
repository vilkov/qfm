#ifndef FILESYSTEMFOLDERCOPYACTION_H_
#define FILESYSTEMFOLDERCOPYACTION_H_

#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderCopyAction : public SyncFileAction
{
public:
	FolderCopyAction();

	virtual const QAction *action() const;
	virtual void process(const FilesList &files) const;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERCOPYACTION_H_ */
