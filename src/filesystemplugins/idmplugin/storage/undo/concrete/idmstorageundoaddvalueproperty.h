#ifndef IDMSTORAGEUNDOADDVALUEPROPERTY_H_
#define IDMSTORAGEUNDOADDVALUEPROPERTY_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddValueProperty : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddValueProperty(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmCompositeEntityValue *m_entityValue;
	IdmEntityValue *m_propertyValue;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOADDVALUEPROPERTY_H_ */
