#ifndef IDM_STORAGE_UNDORENAMEPROPERTY_H_
#define IDM_STORAGE_UNDORENAMEPROPERTY_H_

#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoRenameProperty : public StorageUndoCommand
{
public:
	StorageUndoRenameProperty(Entity *entity, Entity *property);

	virtual void undo(RootEntity &root);

private:
	Entity *m_entity;
	Entity *m_property;
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDORENAMEPROPERTY_H_ */
