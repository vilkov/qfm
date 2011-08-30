#ifndef IDMSTORAGEUNDOREMOVEENTITY_H_
#define IDMSTORAGEUNDOREMOVEENTITY_H_

#include "../idmstorageundocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveEntity : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRemoveEntity(IdmEntity *entity);
	virtual ~IdmStorageUndoRemoveEntity();

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOREMOVEENTITY_H_ */
