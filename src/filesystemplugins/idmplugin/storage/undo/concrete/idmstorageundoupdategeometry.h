#ifndef IDMSTORAGEUNDOUPDATEGEOMETRY_H_
#define IDMSTORAGEUNDOUPDATEGEOMETRY_H_

#include "../idmstorageundocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoUpdateGeometry : public IdmStorageUndoCommand
{
public:
	typedef void (IdmEntity::*Method)(const QRect &);

public:
	IdmStorageUndoUpdateGeometry(IdmEntity *entity, Method method, const QRect &geometry);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
	Method m_method;
	QRect m_geometry;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOUPDATEGEOMETRY_H_ */
