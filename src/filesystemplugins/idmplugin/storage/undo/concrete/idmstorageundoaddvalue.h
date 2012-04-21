#ifndef IDMSTORAGEUNDOADDVALUE_H_
#define IDMSTORAGEUNDOADDVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


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

#endif /* IDMSTORAGEUNDOADDVALUE_H_ */
