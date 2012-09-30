#ifndef IDM_STORAGE_UNDOADDPROPERTY_H_
#define IDM_STORAGE_UNDOADDPROPERTY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoAddProperty : public StorageUndoCommand
{
public:
	StorageUndoAddProperty(Entity *entity, Entity *property);

	virtual void undo(RootEntity &root);

private:
	Entity *m_entity;
	Entity *m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDPROPERTY_H_ */
