#include "vfs_asyncfileaction.h"


VFS_NS_BEGIN

AsyncFileAction::AsyncFileAction(const QIcon &icon, const QString &text) :
	FileAction(icon, text)
{}

bool AsyncFileAction::isAsynchronous() const
{
	return true;
}

VFS_NS_END
