#include "libunrarunpackintosubdiractiontask.h"
#include "../../interfaces/libunrarfilecontainer.h"

#include <wchar.h>
#include <libunrar/rar.hpp>
#include <libunrar/dll.hpp>
#include <libunrar/timefn.hpp>

#include <QtGui/QMessageBox>


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirActionTask::UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files) :
	PerformActionTask(receiver, files),
	m_file(NULL),
	m_container(container),
	m_aborted(NULL)
{}

void UnPackIntoSubdirActionTask::process(const volatile Flags &aborted, QString &error)
{
	QByteArray fileName;
	IFileContainer::Holder destination;

	int res;
	void *archive;
	struct RARHeaderDataEx fileInfo;
	struct RAROpenArchiveDataEx archiveData;

	Tryier tryier(this, &UnPackIntoSubdirActionTask::askForSkipIfNotCopy, aborted);
	Questioner questioner(this, &UnPackIntoSubdirActionTask::askForOverwrite, aborted);

	m_aborted = &aborted;

	for (AsyncFileAction::FilesList::size_type i = 0, size = files().size(); i < size && !aborted; ++i)
		if (tryier.tryTo(CreateDestination(m_container, m_file = files().at(i).second, destination)))
		{
			fileName = m_container->location(m_file->fileName()).toUtf8();

			memset(&archiveData, 0, sizeof(struct RAROpenArchiveDataEx));

			archiveData.ArcName = fileName.data();
			archiveData.OpenMode = RAR_OM_EXTRACT;

			if (archive = RAROpenArchiveEx(&archiveData))
			{
			    RARSetCallback(archive, callbackProc, (long)this);

			    while ((res = RARReadHeaderEx(archive, &fileInfo)) == 0 && !aborted)
			    {
					if ((res = RARProcessFile(archive, RAR_EXTRACT, destination->location().toUtf8().data(), NULL)) != 0)
						break;
			    }

				if (!aborted && res != ERAR_END_ARCHIVE)
		    		error = errorDescription(res);

				RARCloseArchive(archive);
			}
		}
}

bool UnPackIntoSubdirActionTask::CreateDestination::operator()(QString &error) const
{
	return m_result = m_container->open(FileContainer::extractDirectoryName(m_file), true, error);
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

QString UnPackIntoSubdirActionTask::errorDescription(int res) const
{
	switch (res)
	{
		case ERAR_NO_MEMORY:
			return tr("ERAR_NO_MEMORY");

		case ERAR_BAD_DATA:
			return tr("ERAR_BAD_DATA");

		case ERAR_BAD_ARCHIVE:
			return tr("ERAR_BAD_ARCHIVE");

		case ERAR_UNKNOWN_FORMAT:
			return tr("ERAR_UNKNOWN_FORMAT");

		case ERAR_EOPEN:
			return tr("ERAR_EOPEN");

		case ERAR_ECREATE:
			return tr("ERAR_ECREATE");

		case ERAR_ECLOSE:
			return tr("ERAR_ECLOSE");

		case ERAR_EREAD:
			return tr("ERAR_EREAD");

		case ERAR_EWRITE:
			return tr("ERAR_EWRITE");

		case ERAR_SMALL_BUF:
			return tr("ERAR_SMALL_BUF");

		case ERAR_UNKNOWN:
			return tr("ERAR_UNKNOWN");

		default:
			return tr("Unknown");
	}
}

int UnPackIntoSubdirActionTask::callbackProc(unsigned int msg, long userData, long rarBuffer, long bytesProcessed)
{
	UnPackIntoSubdirActionTask *self = static_cast<UnPackIntoSubdirActionTask *>((void *)userData);

	if (*self->m_aborted)
		return -1;
	else
		if (msg == UCM_NEEDPASSWORD)
		{
			QString password;

			if (self->askForUserInput(
					tr("Password protection"),
					tr("Archive \"%1\" is protected by password.\nEnter password to unpack it:").
					arg(self->m_file->fileName()),
					QMessageBox::Ok | QMessageBox::Cancel,
					password,
					*self->m_aborted) == QMessageBox::Ok)
			{
				QByteArray pwd = password.mid(0, bytesProcessed).toUtf8();
				memcpy((void *)rarBuffer, pwd.constData(), pwd.size());
			}
			else
				return -1;
		}

	return 0;
}

LIBUNRAR_ARC_PLUGIN_NS_END
