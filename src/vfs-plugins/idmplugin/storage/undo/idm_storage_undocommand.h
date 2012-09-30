#ifndef IDM_STORAGE_UNDOCOMMAND_H_
#define IDM_STORAGE_UNDOCOMMAND_H_

#include "../entities/idm_rootentity.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoCommand
{
public:
	virtual ~StorageUndoCommand();
	virtual void undo(RootEntity &root) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOCOMMAND_H_ */
