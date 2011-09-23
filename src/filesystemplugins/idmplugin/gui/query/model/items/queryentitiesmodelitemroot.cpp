#include "queryentitiesmodelitemroot.h"


IDM_PLUGIN_NS_BEGIN

QueryEntitiesModelItemRoot::QueryEntitiesModelItemRoot(IdmItem *parent) :
	QueryEntitiesModelItem(QString(), 0, parent)
{}

bool QueryEntitiesModelItemRoot::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
