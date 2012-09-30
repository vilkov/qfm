#ifndef IDM_STORAGE_UNDOADDENTITY_H_
#define IDM_STORAGE_UNDOADDENTITY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoAddEntity : public StorageUndoCommand
{
public:
	StorageUndoAddEntity(Entity *entity);

	virtual void undo(RootEntity &root);

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDENTITY_H_ */
