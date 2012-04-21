#ifndef IDMSTORAGEUNDOREMOVEVALUE_H_
#define IDMSTORAGEUNDOREMOVEVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


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

#endif /* IDMSTORAGEUNDOREMOVEVALUE_H_ */
