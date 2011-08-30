#include "idmstorageundoaddentity.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoAddEntity::IdmStorageUndoAddEntity(IdmEntity *entity) :
	m_entity(entity)
{}

void IdmStorageUndoAddEntity::undo(IdmEntityRoot &root)
{
	root.remove(m_entity);

	for (IdmEntity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i)->removeParent(m_entity);

	delete m_entity;
}

IDM_PLUGIN_NS_END
