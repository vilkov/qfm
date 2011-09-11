#ifndef FILESYSTEMINODE_H_
#define FILESYSTEMINODE_H_

#include "filesystemicopycontrol.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

class INode : public IFileInfo, public IFileOperations, public IFileNavigation
{
public:
	virtual ~INode() {}

	virtual INode *root() const = 0;
	virtual int columnsCount() const = 0;
	virtual IFileInfo *info(const QModelIndex &idx) const = 0;
	virtual ICopyControl *createControl() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODE_H_ */
