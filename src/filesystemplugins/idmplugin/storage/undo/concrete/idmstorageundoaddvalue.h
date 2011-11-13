#ifndef IDMSTORAGEUNDOADDVALUE_H_
#define IDMSTORAGEUNDOADDVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoAddValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoAddValue(IdmEntityValue *value);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOADDVALUE_H_ */
