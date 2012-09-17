#include "arc_libunrar_unpackintosubdiractiontask.h"
#include "../../interfaces/arc_libunrar_scanner.h"
#include "../../interfaces/arc_libunrar_filecontainer.h"

#include <wchar.h>
#include <libunrar/rar.hpp>
#include <libunrar/dll.hpp>
#include <libunrar/timefn.hpp>

#include <QtGui/QMessageBox>


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

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
	Tryier tryier(this, &UnPackIntoSubdirActionTask::askForSkipIfNotCopy, aborted);

	int res;
	void *archive;
	struct RARHeaderDataEx fileInfo;
	struct RAROpenArchiveDataEx archiveData;

	m_aborted = &aborted;

	for (AsyncFileAction::FilesList::size_type i = 0, size = files().size(); i < size && !aborted; ++i)
		if (tryier.tryTo(CreateDestination(m_container, m_file = files().at(i).second, destination)))
		{
			fileName = m_container->location(m_file);

			memset(&archiveData, 0, sizeof(struct RAROpenArchiveDataEx));

			archiveData.ArcName = fileName.data();
			archiveData.OpenMode = RAR_OM_EXTRACT;

			if (archive = RAROpenArchiveEx(&archiveData))
			{
			    RARSetCallback(archive, callbackProc, (long)this);

			    while ((res = RARReadHeaderEx(archive, &fileInfo)) == 0 && !aborted)
			    {
					if ((res = RARProcessFile(archive, RAR_EXTRACT, const_cast<char *>(destination->location().as<QByteArray>().data()), NULL)) != 0)
						break;
			    }

				if (!aborted && res != ERAR_END_ARCHIVE)
		    		error = Scanner::errorDescription(res);

				RARCloseArchive(archive);
			}
		}
}

bool UnPackIntoSubdirActionTask::CreateDestination::operator()(QString &error) const
{
	IFileInfo::Holder file;
	QString name = FileContainer::extractDirectoryName(m_file);

	if ((file = m_container->info(name, error)) && !file->isDir())
	{
		int i = 0;
		QString tmp;

		do
			tmp = QString(name).append(QChar(L' ')).append(QString::number(++i));
		while ((file = m_container->info(tmp, error)) && !file->isDir());

		name = tmp;
	}

	return m_result = m_container->create(name, error);
}

bool UnPackIntoSubdirActionTask::askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						QString(error).append(L'\n').append(tr("Skip it?")),
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

ARC_PLUGIN_LIBUNRAR_NS_END
