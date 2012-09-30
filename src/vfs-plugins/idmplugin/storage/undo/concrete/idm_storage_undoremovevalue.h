#ifndef IDM_STORAGE_UNDOREMOVEVALUE_H_
#define IDM_STORAGE_UNDOREMOVEVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoRemoveValue : public StorageUndoCommand
{
public:
	StorageUndoRemoveValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue);

	virtual void undo(RootEntity &root);

private:
	EntityValue::Holder m_entityValue;
	EntityValue::Holder m_propertyValue;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEVALUE_H_ */
