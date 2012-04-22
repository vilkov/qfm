#ifndef DEFAULTFOLDERPASTECLIPBOARDACTION_H_
#define DEFAULTFOLDERPASTECLIPBOARDACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/synchronous/filesystemsyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderPasteClipboardAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteClipboardAction)

public:
	FolderPasteClipboardAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERPASTECLIPBOARDACTION_H_ */
