#ifndef IDMSTORAGEUNDOADDVALUE_H_
#define IDMSTORAGEUNDOADDVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue);
	IdmStorageUndoAddValue(IdmCompositeEntityValue *entityValue, const IdmCompositeEntityValue::List &propertyValues);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmCompositeEntityValue *m_entityValue;
	IdmCompositeEntityValue::List m_propertyValues;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOADDVALUE_H_ */
