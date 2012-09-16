#ifndef IDM_STORAGE_UNDOCOMMAND_H_
#define IDM_STORAGE_UNDOCOMMAND_H_

#include "../entities/idm_entityroot.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoCommand
{
public:
	virtual ~IdmStorageUndoCommand();
	virtual void undo(IdmEntityRoot &root) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOCOMMAND_H_ */
