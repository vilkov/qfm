#ifndef IDM_STORAGE_UNDOREMOVEVALUE_H_
#define IDM_STORAGE_UNDOREMOVEVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRemoveValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntityValue::Holder m_entityValue;
	IdmEntityValue::Holder m_propertyValue;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEVALUE_H_ */
