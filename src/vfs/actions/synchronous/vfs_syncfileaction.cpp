#include "vfs_syncfileaction.h"


VFS_NS_BEGIN

SyncFileAction::SyncFileAction(const QIcon &icon, const QString &text) :
	FileAction(icon, text)
{}

bool SyncFileAction::isAsynchronous() const
{
	return false;
}

VFS_NS_END
