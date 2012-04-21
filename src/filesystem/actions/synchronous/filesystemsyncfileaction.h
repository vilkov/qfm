#ifndef FILESYSTEMSYNCFILEACTION_H_
#define FILESYSTEMSYNCFILEACTION_H_

#include "../filesystemfileaction.h"


FILE_SYSTEM_NS_BEGIN

class SyncFileAction : public FileAction
{
public:
	SyncFileAction(const QIcon &icon, const QString &text);

	virtual bool isAsynchronous() const;
	virtual void process(const IFileContainer *container, const FilesList &files) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSYNCFILEACTION_H_ */
