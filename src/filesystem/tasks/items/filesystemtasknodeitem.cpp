#include "filesystemtasknodeitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskNodeItem::TaskNodeItem(Base *parent) :
	FileSystemItem(parent)
{}

void TaskNodeItem::lock(const QString &reason)
{
	MODEL_ITEM_LOCKER_LOCK(reason, Application::desktopEnvironment()->processingIcon(16));
}

FILE_SYSTEM_NS_END
