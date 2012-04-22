#ifndef DEFAULTFOLDERPROPERTIESACTION_H_
#define DEFAULTFOLDERPROPERTIESACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderPropertiesAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPropertiesAction)

public:
	FolderPropertiesAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERPROPERTIESACTION_H_ */
