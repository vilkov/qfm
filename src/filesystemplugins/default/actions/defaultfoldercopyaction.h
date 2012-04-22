#ifndef DEFAULTFOLDERCOPYACTION_H_
#define DEFAULTFOLDERCOPYACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderCopyAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCopyAction)

public:
	FolderCopyAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERCOPYACTION_H_ */
