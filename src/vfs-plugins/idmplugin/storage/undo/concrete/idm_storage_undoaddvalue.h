#ifndef IDM_STORAGE_UNDOADDVALUE_H_
#define IDM_STORAGE_UNDOADDVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoAddValue : public StorageUndoCommand
{
public:
	StorageUndoAddValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue);
	StorageUndoAddValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues);

	virtual void undo(RootEntity &root);

private:
	EntityValue::Holder m_entityValue;
	CompositeEntityValue::List m_propertyValues;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDVALUE_H_ */
