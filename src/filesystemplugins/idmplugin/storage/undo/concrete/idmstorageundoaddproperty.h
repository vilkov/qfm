#ifndef IDMSTORAGEUNDOADDPROPERTY_H_
#define IDMSTORAGEUNDOADDPROPERTY_H_

#include "../idmstorageundocommand.h"


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

#endif /* IDMSTORAGEUNDOADDPROPERTY_H_ */
