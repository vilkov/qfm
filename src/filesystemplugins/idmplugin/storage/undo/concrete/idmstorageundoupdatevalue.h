#ifndef IDMSTORAGEUNDOUPDATEVALUE_H_
#define IDMSTORAGEUNDOUPDATEVALUE_H_

#include "../idmstorageundocommand.h"
#include "../../values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoUpdateValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoUpdateValue(IdmEntityValue *value);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntityValue *m_value;
	QVariant m_oldValue;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOUPDATEVALUE_H_ */
