#include "arcunpackintosubdiraction.h"
#include "../archive/arcarchive.h"
#include "../../../application.h"
#include "../../../tools/pointers/pscopedpointer.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"))
{}

bool UnPackIntoSubdirAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackIntoSubdirAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{
	QString error;
	Archive::State *state;
	const IFileControl *file;

	for (FilesList::size_type i = 0, size = files.size(); i < size && !flags; ++i)
	{
		file = files.at(i).second;

		if (const Archive *archive = Archive::archive(file->absoluteFilePath(), &state))
		{
			PScopedPointer<IFileControl> folder(file->openFolder(folderName(file->fileName()), true, error));
			sleep(10);


//			event->contents = archive->readAll(state, aborted);
//			archive->endRead(state);
		}
	}
}

QString UnPackIntoSubdirAction::folderName(const QString &fileName) const
{
	return fileName.mid(0, fileName.indexOf(QChar('.')));
}

ARC_PLUGIN_NS_END
