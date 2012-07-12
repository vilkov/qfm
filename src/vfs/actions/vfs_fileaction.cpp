#include "vfs_fileaction.h"


VFS_NS_BEGIN

FileAction::FileAction(const QIcon &icon, const QString &text) :
	m_action(icon, text, NULL)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

FileAction::~FileAction()
{}

const FileAction *FileAction::fromQAction(const QAction *action)
{
	if (action)
		return static_cast<const FileAction *>(action->data().value<void *>());
	else
		return 0;
}

FileAction *FileAction::fromQAction(QAction *action)
{
	if (action)
		return static_cast<FileAction *>(action->data().value<void *>());
	else
		return 0;
}

VFS_NS_END
