#include "idm_storage_undorenameproperty.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoRenameProperty::StorageUndoRenameProperty(Entity *entity, Entity *property) :
	m_entity(entity),
	m_property(property),
	m_name(entity->at(entity->indexOf(property)).name)
{}

void StorageUndoRenameProperty::undo(RootEntity &root)
{
	m_entity->rename(m_property, m_name);
}

IDM_PLUGIN_NS_END
