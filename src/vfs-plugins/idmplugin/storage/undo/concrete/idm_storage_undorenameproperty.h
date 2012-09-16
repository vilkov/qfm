#ifndef IDM_STORAGE_UNDORENAMEPROPERTY_H_
#define IDM_STORAGE_UNDORENAMEPROPERTY_H_

#include "../idm_storage_undocommand.h"


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

#endif /* IDM_STORAGE_UNDORENAMEPROPERTY_H_ */
