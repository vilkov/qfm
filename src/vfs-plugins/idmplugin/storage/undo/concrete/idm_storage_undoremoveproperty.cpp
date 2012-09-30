#include "idm_storage_undoremoveproperty.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoRemoveProperty::StorageUndoRemoveProperty(Entity *entity, Entity *property, const QString &name) :
	m_entity(entity),
	m_property(property),
	m_name(name)
{}

void StorageUndoRemoveProperty::undo(RootEntity &root)
{
	m_entity->add(m_property, m_name);
	m_property->addParent(m_entity);
}

IDM_PLUGIN_NS_END
