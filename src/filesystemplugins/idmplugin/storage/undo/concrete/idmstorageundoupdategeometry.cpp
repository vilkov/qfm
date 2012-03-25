#include "idmstorageundoupdategeometry.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoUpdateGeometry::IdmStorageUndoUpdateGeometry(IdmEntity *entity, Method method, const QRect &geometry) :
	m_entity(entity),
	m_method(method),
	m_geometry(geometry)
{}

void IdmStorageUndoUpdateGeometry::undo(IdmEntityRoot &root)
{
	(m_entity->*m_method)(m_geometry);
}

IDM_PLUGIN_NS_END
