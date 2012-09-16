#include "idm_storage_undoaddproperty.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoAddProperty::IdmStorageUndoAddProperty(IdmEntity *entity, IdmEntity *property) :
	m_entity(entity),
	m_property(property)
{}

void IdmStorageUndoAddProperty::undo(IdmEntityRoot &root)
{
	m_property->removeParent(m_entity);
	m_entity->remove(m_property);
}

IDM_PLUGIN_NS_END
