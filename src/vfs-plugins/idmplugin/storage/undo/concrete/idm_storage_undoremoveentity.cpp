#include "idm_storage_undoremoveentity.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoRemoveEntity::StorageUndoRemoveEntity(Entity *entity) :
	m_entity(entity)
{}

StorageUndoRemoveEntity::~StorageUndoRemoveEntity()
{
	delete m_entity;
}

void StorageUndoRemoveEntity::undo(RootEntity &root)
{
	root.add(m_entity, m_entity->name());

	for (Parents::size_type i = 0, size = m_parents.size(); i < size; ++i)
		m_parents.at(i).entity->add(m_entity, m_parents.at(i).name);

	for (Entity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i).entity->addParent(m_entity);

	m_entity = 0;
}

IDM_PLUGIN_NS_END
