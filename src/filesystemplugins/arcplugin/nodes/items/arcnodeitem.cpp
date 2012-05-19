#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeItem::ArcNodeItem(Base *parent) :
	TasksNodeItem(parent)
{}

void ArcNodeItem::lock(const QString &reason)
{
	TasksNodeItem::lock(reason);
}

bool ArcNodeItem::isRoot() const
{
	return false;
}

ARC_PLUGIN_NS_END
