#ifndef IDMSTORAGEUNDOCOMMAND_H_
#define IDMSTORAGEUNDOCOMMAND_H_

#include "../entities/idmentityroot.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoCommand
{
public:
	virtual ~IdmStorageUndoCommand();
	virtual void undo(IdmEntityRoot &root) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGEUNDOCOMMAND_H_ */
