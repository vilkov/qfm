#ifndef VFS_ICOPYCONTROL_H_
#define VFS_ICOPYCONTROL_H_

#include "vfs_ifilecontainer.h"
#include "../containers/vfs_snapshot.h"


VFS_NS_BEGIN
class INode;


class ICopyControl : public IFileContainer
{
public:
	typedef PScopedPointer<ICopyControl> Holder;

public:
	virtual INode *node() const = 0;

	virtual bool start(const Snapshot &files, bool move) = 0;
	virtual void done(bool error) = 0;
	virtual void canceled() = 0;
};

VFS_NS_END

#endif /* VFS_ICOPYCONTROL_H_ */
