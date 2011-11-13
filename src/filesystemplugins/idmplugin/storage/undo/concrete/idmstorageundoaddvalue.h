#ifndef IDMSTORAGEUNDOADDVALUE_H_
#define IDMSTORAGEUNDOADDVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmCompositeEntityValue *m_entityValue;
	IdmEntityValue *m_propertyValue;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOADDVALUE_H_ */
