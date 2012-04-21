#ifndef IDMSTORAGEUNDOADDENTITY_H_
#define IDMSTORAGEUNDOADDENTITY_H_

#include "../idmstorageundocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddEntity : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddEntity(IdmEntity *entity);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOADDENTITY_H_ */
