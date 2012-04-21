#ifndef FILESYSTEMFOLDERPROPERTIESACTION_H_
#define FILESYSTEMFOLDERPROPERTIESACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPropertiesAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPropertiesAction)

public:
	FolderPropertiesAction();

	virtual void process(const IFileContainer *container, const FilesList &files) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPROPERTIESACTION_H_ */
