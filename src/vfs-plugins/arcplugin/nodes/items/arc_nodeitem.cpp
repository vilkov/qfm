#include "arc_nodeitem.h"


ARC_PLUGIN_NS_BEGIN

NodeItem::NodeItem(Base *parent) :
	TasksNodeItem(parent)
{}

void NodeItem::lock(const QString &reason)
{
	TasksNodeItem::lock(reason);
}

bool NodeItem::isRoot() const
{
	return false;
}

ARC_PLUGIN_NS_END
