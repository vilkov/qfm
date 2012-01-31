#ifndef FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_
#define FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteIntoFolderAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteIntoFolderAction)

public:
	FolderPasteIntoFolderAction();

	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_ */
