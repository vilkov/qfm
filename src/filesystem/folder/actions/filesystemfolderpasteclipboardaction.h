#ifndef FILESYSTEMFOLDERPASTECLIPBOARDACTION_H_
#define FILESYSTEMFOLDERPASTECLIPBOARDACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteClipboardAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteClipboardAction)

public:
	FolderPasteClipboardAction();

	virtual const QAction *action() const;
	virtual void process(const FilesList &files) const;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTECLIPBOARDACTION_H_ */
