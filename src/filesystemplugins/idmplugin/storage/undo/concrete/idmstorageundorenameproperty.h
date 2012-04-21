#ifndef IDMSTORAGEUNDORENAMEPROPERTY_H_
#define IDMSTORAGEUNDORENAMEPROPERTY_H_

#include "../idmstorageundocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRenameProperty : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRenameProperty(IdmEntity *entity, IdmEntity *property);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
	IdmEntity *m_property;
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDORENAMEPROPERTY_H_ */
