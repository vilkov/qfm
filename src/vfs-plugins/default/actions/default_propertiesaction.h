#ifndef DEFAULT_PROPERTIESACTION_H_
#define DEFAULT_PROPERTIESACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class PropertiesAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(PropertiesAction)

public:
	PropertiesAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PROPERTIESACTION_H_ */
