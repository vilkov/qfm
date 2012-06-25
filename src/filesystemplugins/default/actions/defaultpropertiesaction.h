#ifndef DEFAULTPROPERTIESACTION_H_
#define DEFAULTPROPERTIESACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class PropertiesAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(PropertiesAction)

public:
	PropertiesAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPROPERTIESACTION_H_ */
