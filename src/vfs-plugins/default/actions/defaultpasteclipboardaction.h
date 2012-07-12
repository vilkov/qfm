#ifndef DEFAULTPASTECLIPBOARDACTION_H_
#define DEFAULTPASTECLIPBOARDACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class PasteClipboardAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(PasteClipboardAction)

public:
	PasteClipboardAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPASTECLIPBOARDACTION_H_ */
