#include "idmstorageundoremoveentity.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRemoveEntity::IdmStorageUndoRemoveEntity(IdmEntity *entity) :
	m_entity(entity)
{}

IdmStorageUndoRemoveEntity::~IdmStorageUndoRemoveEntity()
{
	delete m_entity;
}

void IdmStorageUndoRemoveEntity::undo(IdmEntityRoot &root)
{
	root.add(m_entity);

	for (IdmEntity::Parents::size_type i = 0, size = m_entity->parents().size(); i < size; ++i)
		m_entity->parents().at(i)->add(m_entity);

	for (IdmEntity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i)->addParent(m_entity);

	m_entity = 0;
}

IDM_PLUGIN_NS_END
