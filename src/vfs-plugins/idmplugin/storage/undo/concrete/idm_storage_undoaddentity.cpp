#include "idm_storage_undoaddentity.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoAddEntity::StorageUndoAddEntity(Entity *entity) :
	m_entity(entity)
{}

void StorageUndoAddEntity::undo(RootEntity &root)
{
	root.remove(m_entity);

	for (Entity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i).entity->removeParent(m_entity);

	delete m_entity;
}

IDM_PLUGIN_NS_END
