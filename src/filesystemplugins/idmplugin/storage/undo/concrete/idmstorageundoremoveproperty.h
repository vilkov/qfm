#ifndef IDMSTORAGEUNDOREMOVEPROPERTY_H_
#define IDMSTORAGEUNDOREMOVEPROPERTY_H_

#include "../idmstorageundocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveProperty : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRemoveProperty(IdmEntity *entity, IdmEntity *property);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
	IdmEntity *m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOREMOVEPROPERTY_H_ */
