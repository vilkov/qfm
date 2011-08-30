#include "idmstorageundoremoveproperty.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRemoveProperty::IdmStorageUndoRemoveProperty(IdmEntity *entity, IdmEntity *property) :
	m_entity(entity),
	m_property(property)
{}

void IdmStorageUndoRemoveProperty::undo(IdmEntityRoot &root)
{
	m_entity->add(m_property);
	m_property->addParent(m_entity);
}

IDM_PLUGIN_NS_END
