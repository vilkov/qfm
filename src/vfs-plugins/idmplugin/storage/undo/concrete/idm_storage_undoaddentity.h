#ifndef IDM_STORAGE_UNDOADDENTITY_H_
#define IDM_STORAGE_UNDOADDENTITY_H_

#include "../idm_storage_undocommand.h"


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

#endif /* IDM_STORAGE_UNDOADDENTITY_H_ */
