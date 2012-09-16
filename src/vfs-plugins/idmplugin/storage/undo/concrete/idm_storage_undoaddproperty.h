#ifndef IDM_STORAGE_UNDOADDPROPERTY_H_
#define IDM_STORAGE_UNDOADDPROPERTY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddProperty : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddProperty(IdmEntity *entity, IdmEntity *property);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
	IdmEntity *m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDPROPERTY_H_ */
