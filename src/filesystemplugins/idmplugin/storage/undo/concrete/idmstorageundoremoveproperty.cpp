#include "idmstorageundoremoveproperty.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRemoveProperty::IdmStorageUndoRemoveProperty(IdmEntity *entity, IdmEntity *property, const QString &name) :
	m_entity(entity),
	m_property(property),
	m_name(name)
{}

void IdmStorageUndoRemoveProperty::undo(IdmEntityRoot &root)
{
	m_entity->add(m_property, m_name);
	m_property->addParent(m_entity);
}

IDM_PLUGIN_NS_END
