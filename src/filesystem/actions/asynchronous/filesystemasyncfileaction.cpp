#include "filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

AsyncFileAction::AsyncFileAction(const QIcon &icon, const QString &text) :
	FileAction(icon, text)
{}

bool AsyncFileAction::isAsynchronous() const
{
	return true;
}

FILE_SYSTEM_NS_END
