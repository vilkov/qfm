#include "filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

bool AsyncFileAction::isAsynchronous() const
{
	return true;
}

FILE_SYSTEM_NS_END
