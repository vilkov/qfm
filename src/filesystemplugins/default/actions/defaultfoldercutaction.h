#ifndef DEFAULTMFOLDERCUTACTION_H_
#define DEFAULTMFOLDERCUTACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderCutAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCutAction)

public:
	FolderCutAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTMFOLDERCUTACTION_H_ */
