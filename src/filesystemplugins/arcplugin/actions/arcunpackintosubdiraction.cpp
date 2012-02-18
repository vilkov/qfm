#include "arcunpackintosubdiraction.h"
#include "../archive/arcarchive.h"
#include "../../../filesystem/tasks/perform/performactiontask.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

class ArchiveCallback : public Archive::Callback
{
public:
	typedef BaseTask::Flags Flags;
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

public:
	ArchiveCallback()
	{}

	virtual IFile::value_type *buffer() const { return const_cast<IFile::value_type *>(m_buffer); }
	virtual IFile::size_type bufferSize() const { return FileReadWriteGranularity; }

	virtual void progressInit(const FileSystemItem *item) {}
	virtual void progressUpdate(quint64 progressIncrement) {}
	virtual void progresscomplete() {}

	virtual bool overwriteAll() const { return false; }
	virtual bool skipAllIfNotCopy() const { return false; }
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted) {}
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted) {}

private:
	IFile::value_type m_buffer[FileReadWriteGranularity];
};


UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"))
{}

bool UnPackIntoSubdirAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackIntoSubdirAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &aborted) const
{
	QString error;
	Archive::State *state;
	const IFileControl *file;
	PScopedPointer<ArchiveCallback> callback(new ArchiveCallback());

	for (FilesList::size_type i = 0, size = files.size(); i < size && !aborted; ++i)
	{
		file = files.at(i).second;

		if (const Archive *archive = Archive::archive(file->absoluteFilePath(), &state))
		{
			PScopedPointer<IFileControl> folder(file->openFolder(folderName(file->fileName()), true, error));

			archive->extractAll(state, folder.data(), callback.data(), aborted);

			archive->endRead(state);
		}
	}
}

QString UnPackIntoSubdirAction::folderName(const QString &fileName) const
{
	return fileName.mid(0, fileName.indexOf(QChar('.')));
}

ARC_PLUGIN_NS_END
