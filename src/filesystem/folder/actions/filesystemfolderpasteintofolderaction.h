#ifndef FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_
#define FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteIntoFolderAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteIntoFolderAction)

public:
	FolderPasteIntoFolderAction();

	virtual const QAction *action() const;
	virtual void process(const FilesList &files) const;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_ */
