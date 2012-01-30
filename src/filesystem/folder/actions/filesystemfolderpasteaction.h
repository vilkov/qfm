#ifndef FILESYSTEMFOLDERPASTEACTION_H_
#define FILESYSTEMFOLDERPASTEACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteAction)

public:
	FolderPasteAction();

	virtual void process(const FilesList &files) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEACTION_H_ */
