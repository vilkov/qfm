#ifndef IDM_STORAGE_UNDOUPDATEVALUE_H_
#define IDM_STORAGE_UNDOUPDATEVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoUpdateValue : public IdmStorageUndoCommand
{
public:
	IdmStorageUndoUpdateValue(const IdmEntityValue::Holder &value);

	virtual void undo(IdmEntityRoot &root);

private:
	IdmEntityValue::Holder m_value;
	QVariant m_oldValue;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOUPDATEVALUE_H_ */
