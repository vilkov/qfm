#include "filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

SyncFileAction::SyncFileAction(const QIcon &icon, const QString &text) :
	FileAction(icon, text)
{}

bool SyncFileAction::isAsynchronous() const
{
	return false;
}

FILE_SYSTEM_NS_END
