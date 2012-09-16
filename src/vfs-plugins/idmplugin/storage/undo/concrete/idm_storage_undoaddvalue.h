#ifndef IDM_STORAGE_UNDOADDVALUE_H_
#define IDM_STORAGE_UNDOADDVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue);
	IdmStorageUndoAddValue(const IdmEntityValue::Holder &entityValue, const IdmCompositeEntityValue::List &propertyValues);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntityValue::Holder m_entityValue;
	IdmCompositeEntityValue::List m_propertyValues;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDVALUE_H_ */
