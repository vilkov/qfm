#include "filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

bool SyncFileAction::isAsynchronous() const
{
	return false;
}

FILE_SYSTEM_NS_END
