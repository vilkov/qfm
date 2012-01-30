#ifndef IDMCREATEDBACTION_H_
#define IDMCREATEDBACTION_H_

#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


IDM_PLUGIN_NS_BEGIN

class CreateDbAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCopyAction)

public:
	CreateDbAction();

	virtual void process(const FilesList &files) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMCREATEDBACTION_H_ */
