#ifndef FILESYSTEMINODE_H_
#define FILESYSTEMINODE_H_

#include "filesystemifilecontrol.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class INode : public IFileInfo, public IFileOperations, public IFileNavigation
{
public:
	virtual ~INode() {}

	virtual INode *root() const = 0;
	virtual int columnCount() const = 0;
	virtual IFileInfo *info(const QModelIndex &idx) const = 0;
	virtual IFileControl *createControl() const = 0;
	virtual IFileControl *createControl(const QModelIndex &idx, PluginsManager *plugins) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODE_H_ */
