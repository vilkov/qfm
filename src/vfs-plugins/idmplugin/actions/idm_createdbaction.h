#ifndef IDM_CREATEDBACTION_H_
#define IDM_CREATEDBACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class CreateDbAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCopyAction)

public:
	CreateDbAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CREATEDBACTION_H_ */
