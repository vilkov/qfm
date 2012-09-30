#ifndef IDM_STORAGE_UNDOREMOVEPROPERTY_H_
#define IDM_STORAGE_UNDOREMOVEPROPERTY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoRemoveProperty : public StorageUndoCommand
{
public:
	StorageUndoRemoveProperty(Entity *entity, Entity *property, const QString &name);

	virtual void undo(RootEntity &root);

private:
	Entity *m_entity;
	Entity *m_property;
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEPROPERTY_H_ */
