#ifndef VFS_IFILEOPERATIONS_H_
#define VFS_IFILEOPERATIONS_H_

#include "vfs_inodeview.h"
#include "vfs_icopycontrol.h"


VFS_NS_BEGIN

class IFileOperations
{
public:
	virtual ~IFileOperations();

	virtual ICopyControl *createControl(INodeView *view) const = 0;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view) = 0;
	virtual void cancel(const QModelIndexList &list, INodeView *view) = 0;
};

VFS_NS_END

#endif /* VFS_IFILEOPERATIONS_H_ */
