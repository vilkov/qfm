#include "idm_createdbaction.h"
#include "../storage/idm_storage.h"
#include "../idm_plugin.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

CreateDbAction::CreateDbAction() :
	SyncFileAction(QIcon(), tr("Create database..."))
{}

void CreateDbAction::process(const IFileContainer *container, const FilesList &files) const
{
	QString error;
	IFileInfo::Holder info;
	IFileContainer::Holder folder;

	for (FilesList::size_type i = 0, size = files.size(); i < size; ++i)
		if (files.at(i).second->isDir())
			if (folder = container->open(files.at(i).second, error))
				if (info = folder->info(Plugin::fileName(), error))
				{
					int res = QMessageBox::question(
							Application::mainWindow(),
							tr("Create database"),
							tr("Database already exist in\n\"%1\"\nWould you like to copy data from it?").
								arg(folder->location()),
							QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

					if (res != QMessageBox::Cancel)
					{
						QString oldName = QString(Plugin::fileName()).append(QString::fromLatin1(".old"));

						if (folder->contains(oldName))
						{
							int number = 1;
							QString string = QString(oldName).append(QString::number(number));

							while (folder->contains(string));
							{
								++number;
								string = QString(oldName).append(QString::number(number));
							}

							oldName = string;
						}

						if (folder->rename(info, oldName, error))
							if (res == QMessageBox::Yes)
							{
								IdmStorage storage(folder->location(Plugin::fileName()), folder->location(oldName));

								if (storage.isValid())
									QMessageBox::information(
											Application::mainWindow(),
											tr("Create database"),
											tr("All done"));
								else
									QMessageBox::critical(
											Application::mainWindow(),
											tr("Create database"),
											tr("Error: ").
												append(storage.lastError()));
							}
							else
							{
								IdmStorage storage(folder->location(Plugin::fileName()), true);

								if (storage.isValid())
									QMessageBox::information(
											Application::mainWindow(),
											tr("Create database"),
											tr("All done"));
								else
									QMessageBox::critical(
											Application::mainWindow(),
											tr("Create database"),
											tr("Error: ").
												append(storage.lastError()));
							}
						else
							QMessageBox::critical(
									Application::mainWindow(),
									tr("Create database"),
									tr("Error: ").
										append(error));
					}
				}
				else
				{
					IdmStorage storage(folder->location(Plugin::fileName()), true);

					if (storage.isValid())
						QMessageBox::information(
								Application::mainWindow(),
								tr("Create database"),
								tr("All done"));
					else
						QMessageBox::critical(
								Application::mainWindow(),
								tr("Create database"),
								tr("Error: ").
									append(storage.lastError()));
				}
			else
				QMessageBox::critical(
						Application::mainWindow(),
						tr("Create database"),
						tr("Error: ").
							append(error));
}

IDM_PLUGIN_NS_END
