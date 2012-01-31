#ifndef FILESYSTEMFOLDERPASTEACTION_H_
#define FILESYSTEMFOLDERPASTEACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteAction)

public:
	FolderPasteAction();

	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEACTION_H_ */
