#ifndef VFS_IAPPLICATIONS_H_
#define VFS_IAPPLICATIONS_H_

#include <QtCore/QLinkedList>
#include "vfs_iapplication.h"
#include "vfs_ifiletype.h"


VFS_NS_BEGIN

class IApplications
{
public:
	typedef QLinkedList<const IApplication *> LinkedList;

public:
	virtual ~IApplications();

	virtual LinkedList user(const IFileType *fileType) const = 0;
	virtual LinkedList system(const IFileType *fileType) const = 0;
};

VFS_NS_END

#endif /* VFS_IAPPLICATIONS_H_ */
