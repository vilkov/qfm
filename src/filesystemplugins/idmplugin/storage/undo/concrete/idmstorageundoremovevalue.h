#ifndef IDMSTORAGEUNDOREMOVEVALUE_H_
#define IDMSTORAGEUNDOREMOVEVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoRemoveValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue);
	virtual ~IdmStorageUndoRemoveValue();

	virtual void undo(IdmEntityRoot &root);

private:
	IdmCompositeEntityValue *m_entityValue;
	IdmEntityValue *m_propertyValue;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOREMOVEVALUE_H_ */
