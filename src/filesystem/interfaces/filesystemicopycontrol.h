#ifndef FILESYSTEMICOPYCONTROL_H_
#define FILESYSTEMICOPYCONTROL_H_

#include "filesystemifilecontainer.h"
#include "../containers/filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN
class INode;


class ICopyControl : public IFileContainer
{
public:
	typedef PScopedPointer<ICopyControl> Holder;

public:
	virtual INode *node() const = 0;

	virtual bool start(const Snapshot::Files &files, bool move) = 0;
	virtual void done(bool error) = 0;
	virtual void canceled() = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMICOPYCONTROL_H_ */