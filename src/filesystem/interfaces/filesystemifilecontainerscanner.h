#ifndef FILESYSTEMIFILECONTAINERSCANNER_H_
#define FILESYSTEMIFILECONTAINERSCANNER_H_

#include "../containers/filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

class IFileContainerScanner
{
public:
	virtual ~IFileContainerScanner();

	virtual void scan(Snapshot &snapshot) const = 0;
	virtual void update(Snapshot &snapshot) const = 0;
	virtual void refresh(Snapshot &snapshot) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTAINERSCANNER_H_ */
