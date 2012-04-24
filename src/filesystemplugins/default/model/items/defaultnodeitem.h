#ifndef DEFAULTNODEITEM_H_
#define DEFAULTNODEITEM_H_

#include "../../default_ns.h"
#include "../../../../filesystem/interfaces/filesystemifileinfo.h"
#include "../../../../filesystem/tasks/model/items/filesystemtasksnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultNodeItem : public TasksNodeItem, public IFileInfo
{
public:
	DefaultNodeItem(Base *parent = 0);

	virtual bool isRootItem() const = 0;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTNODEITEM_H_ */
