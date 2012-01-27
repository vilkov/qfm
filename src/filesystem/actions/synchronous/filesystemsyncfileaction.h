#ifndef FILESYSTEMSYNCFILEACTION_H_
#define FILESYSTEMSYNCFILEACTION_H_

#include "../filesystemfileaction.h"
#include "../interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class SyncFileAction : public FileAction
{
public:
	typedef QList<const IFileControl *> FilesList;

public:
	virtual void process(const FilesList &files) const = 0;
};

FILE_SYSTEM_NS_END


#endif /* FILESYSTEMSYNCFILEACTION_H_ */
