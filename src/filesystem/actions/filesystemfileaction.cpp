#include "filesystemfileaction.h"


FILE_SYSTEM_NS_BEGIN

FileAction::FileAction(const QIcon &icon, const QString &text) :
	m_action(icon, text, NULL)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

FileAction::~FileAction()
{}

const FileAction *FileAction::fromAction(const QAction *action)
{
	return static_cast<const FileAction *>(action->data().value<void *>());
}

FILE_SYSTEM_NS_END
