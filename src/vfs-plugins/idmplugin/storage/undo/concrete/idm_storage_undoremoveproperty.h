#ifndef IDM_STORAGE_UNDOREMOVEPROPERTY_H_
#define IDM_STORAGE_UNDOREMOVEPROPERTY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveProperty : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRemoveProperty(IdmEntity *entity, IdmEntity *property, const QString &name);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntity *m_entity;
	IdmEntity *m_property;
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEPROPERTY_H_ */
