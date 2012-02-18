#include "filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

TaskNodeItem::TaskNodeItem(Base *parent) :
	FileSystemItem(parent),
	m_total(0),
	m_timeElapsed(0)
{}

FILE_SYSTEM_NS_END
