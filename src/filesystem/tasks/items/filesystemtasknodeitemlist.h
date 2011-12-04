#ifndef FILESYSTEMTASKNODEITEMLIST_H_
#define FILESYSTEMTASKNODEITEMLIST_H_

#include "filesystemtasknodeitem.h"
#include "../../model/items/filesystemlistitem.h"


FILE_SYSTEM_NS_BEGIN

class TaskNodeListItem : public FileSystemListItem
{
	DECLARE_MODEL_ITEM_LOCKER;

public:
	TaskNodeListItem(Base *parent);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKNODEITEMLIST_H_ */
