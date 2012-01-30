#include "filesystemfolderpasteaction.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteAction::FolderPasteAction() :
	m_action(tr("Paste"), 0)
{}

const QAction *FolderPasteAction::action() const
{
	return &m_action;
}

void FolderPasteAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
