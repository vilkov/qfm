#include "idmstorageundorenameproperty.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRenameProperty::IdmStorageUndoRenameProperty(IdmEntity *entity, IdmEntity *property) :
	m_entity(entity),
	m_property(property),
	m_name(entity->at(entity->indexOf(property)).name)
{}

void IdmStorageUndoRenameProperty::undo(IdmEntityRoot &root)
{
	m_entity->rename(m_property, m_name);
}

IDM_PLUGIN_NS_END
