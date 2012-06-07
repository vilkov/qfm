#include "arcunpackintosubdiractiontask.h"
#include "../../interfaces/libarchivefilecontainer.h"

#include <QtGui/QMessageBox>


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirActionTask::UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files) :
	PerformActionTask(receiver, files),
	m_progress(receiver),
	m_container(container)
{}

void UnPackIntoSubdirActionTask::progressInit(const NodeItem::Holder &item)
{
	m_progress.init(item);
}

void UnPackIntoSubdirActionTask::progressUpdate(quint64 progressIncrement)
{
	m_progress.update(progressIncrement);
}

void UnPackIntoSubdirActionTask::progresscomplete()
{
	m_progress.complete();
}

void UnPackIntoSubdirActionTask::process(const volatile Flags &aborted, QString &error)
{
	QString fileName;
	const IFileInfo *file;
	IFileInfo::Holder info;
	const IFileContainer *dest;
	IFileAccessor::Holder destFile;
	IFileAccessor::Holder sourceFile;
	IFileContainer::Holder archive;
	IFileContainer::Holder destination;
	IFileContainerScanner::IEnumerator::Holder enumerator;
	IFileAccessor::size_type readed;

	Tryier tryier(this, &UnPackIntoSubdirActionTask::askForSkipIfNotCopy, aborted);
	Questioner questioner(this, &UnPackIntoSubdirActionTask::askForOverwrite, aborted);

	for (AsyncFileAction::FilesList::size_type i = 0, size = files().size(); i < size && !aborted; ++i)
		if (tryier.tryTo(OpenArchive(m_container, file = files().at(i).second, archive)))
			if (tryier.tryTo(CreateDestination(m_container, file, destination)))
			{
				FilesTree tree(destination.data(), true);
				archive->scanner()->enumerate(enumerator);

				while (!aborted && enumerator->next())
				{
					info = enumerator->info();

					if (tryier.tryTo(OpenDestination(tree, info.data(), dest)))
						if (info->isFile() &&
							questioner.askFor(OverwriteFile(dest, fileName = FileContainer::extractArchivedFileName(info.data()))) &&
							tryier.tryTo(OpenArchiveFile(enumerator.data(), sourceFile)) &&
							tryier.tryTo(CreateFile(dest, fileName, destFile)))
						{
							for (;!aborted;)
							{
								readed = sourceFile->read(m_buffer, FileReadWriteGranularity);

								if (((int)readed) < 0)
								{
									error = sourceFile->lastError();
									cancel();
									break;
								}

								if (readed == 0)
									break;

								if (destFile->write(m_buffer, readed) != readed)
								{
									error = destFile->lastError();
									cancel();
									break;
								}
							}
						}
				}
			}
}

bool UnPackIntoSubdirActionTask::OpenArchive::operator()(QString &error) const
{
	return m_result = FileContainer::create(m_container, m_file, error);
}

bool UnPackIntoSubdirActionTask::CreateDestination::operator()(QString &error) const
{
	return m_result = m_container->open(FileContainer::extractDirectoryName(m_file), true, error);
}

bool UnPackIntoSubdirActionTask::OpenDestination::operator()(QString &error) const
{
	return m_result = m_tree.open(m_file->fileName(), m_file->isDir(), error);
}

bool UnPackIntoSubdirActionTask::OpenArchiveFile::operator()(QString &error) const
{
	return m_result = m_enumerator->open(IFileAccessor::ReadOnly, error);
}

bool UnPackIntoSubdirActionTask::OverwriteFile::operator()(QString &error) const
{
	if (m_container->contains(m_fileName))
	{
		error = tr("File \"%1\" already exists in \"%2\".").
				arg(m_fileName).
				arg(m_container->location()).
				append(QChar('\n')).
				append(tr("Overwrite it?"));
		return false;
	}
	else
		return true;
}

bool UnPackIntoSubdirActionTask::CreateFile::operator()(QString &error) const
{
	return m_result = m_container->open(m_fileName, IFileAccessor::ReadWrite | IFileAccessor::Create | IFileAccessor::Truncate, error);
}

bool UnPackIntoSubdirActionTask::askForOverwrite(const QString &error, Questioner::Tristate &flag, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						error,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::No |
						QMessageBox::NoToAll |
						QMessageBox::Cancel,
						aborted);

	switch (answer)
	{
		case QMessageBox::Yes:
			return true;
			break;

		case QMessageBox::YesToAll:
			return flag = true;
			break;

		case QMessageBox::No:
			return false;
			break;

		case QMessageBox::NoToAll:
			return flag = false;
			break;

		case QMessageBox::Cancel:
			cancel();
			break;
	}

	return false;
}

bool UnPackIntoSubdirActionTask::askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						QString(error).append('\n').append(tr("Skip it?")),
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::Retry |
						QMessageBox::Cancel,
						aborted);

	if (answer == QMessageBox::YesToAll)
		flag = true;
	else
		if (answer == QMessageBox::Retry)
			return true;
		else
			if (answer == QMessageBox::Cancel)
				cancel();

	return false;
}

LIBARCHIVE_ARC_PLUGIN_NS_END
