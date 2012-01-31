#include "filesystemfolderpasteaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteAction::FolderPasteAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste"))
{}

bool FolderPasteAction::prepare(const FilesList &files)
{
	return false;
}

void FolderPasteAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

FILE_SYSTEM_NS_END
