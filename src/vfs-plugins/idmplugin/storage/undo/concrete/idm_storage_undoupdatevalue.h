#ifndef IDM_STORAGE_UNDOUPDATEVALUE_H_
#define IDM_STORAGE_UNDOUPDATEVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoUpdateValue : public StorageUndoCommand
{
public:
	StorageUndoUpdateValue(const EntityValue::Holder &value);

	virtual void undo(RootEntity &root);

private:
	EntityValue::Holder m_value;
	QVariant m_oldValue;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOUPDATEVALUE_H_ */
