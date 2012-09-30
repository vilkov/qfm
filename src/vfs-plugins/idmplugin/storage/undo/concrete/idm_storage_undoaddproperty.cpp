#include "idm_storage_undoaddproperty.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoAddProperty::StorageUndoAddProperty(Entity *entity, Entity *property) :
	m_entity(entity),
	m_property(property)
{}

void StorageUndoAddProperty::undo(RootEntity &root)
{
	m_property->removeParent(m_entity);
	m_entity->remove(m_property);
}

IDM_PLUGIN_NS_END
