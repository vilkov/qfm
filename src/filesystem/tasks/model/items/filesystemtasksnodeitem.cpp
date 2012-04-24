#include "filesystemtasksnodeitem.h"


FILE_SYSTEM_NS_BEGIN

TasksNodeItem::TasksNodeItem(Base *parent) :
	NodeItem(parent),
	m_total(0),
	m_timeElapsed(0)
{}

FILE_SYSTEM_NS_END
