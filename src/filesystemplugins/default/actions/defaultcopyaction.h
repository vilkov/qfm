#ifndef DEFAULTCOPYACTION_H_
#define DEFAULTCOPYACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class CopyAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(CopyAction)

public:
	CopyAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTCOPYACTION_H_ */
