#ifndef DEFAULTFOLDERITEM_H_
#define DEFAULTFOLDERITEM_H_

#include "../../default_ns.h"
#include "../../../../filesystem/interfaces/filesystemifileinfo.h"
#include "../../../../filesystem/tasks/model/items/filesystemtasknodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultFolderItem : public TaskNodeItem, public IFileInfo
{
public:
	DefaultFolderItem(Base *parent = 0);

	virtual bool isRootItem() const = 0;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERITEM_H_ */
