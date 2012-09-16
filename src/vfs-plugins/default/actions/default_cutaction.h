#ifndef DEFAULT_MCUTACTION_H_
#define DEFAULT_MCUTACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class CutAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(CutAction)

public:
	CutAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_MCUTACTION_H_ */
