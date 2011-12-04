#ifndef FILESYSTEMTASKNODEITEM_H_
#define FILESYSTEMTASKNODEITEM_H_

#include "../../model/items/filesystemitem.h"
#include "../../../tools/models/macros/declareitemlocker.h"


FILE_SYSTEM_NS_BEGIN

class TaskNodeItem : public FileSystemItem
{
	DECLARE_MODEL_ITEM_LOCKER;

public:
	TaskNodeItem(Base *parent);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKNODEITEM_H_ */
