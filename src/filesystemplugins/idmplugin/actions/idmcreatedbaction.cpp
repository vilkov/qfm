#include "idmcreatedbaction.h"
#include "../storage/idmstorage.h"
#include "../idmplugin.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

CreateDbAction::CreateDbAction() :
	SyncFileAction(QIcon(), tr("Create database..."))
{}

void CreateDbAction::process(const IFileContainer *container, const FilesList &files) const
{
	QString error;
	IFileContainer::Holder folder;

	for (FilesList::size_type i = 0, size = files.size(); i < size; ++i)
		if (files.at(i).second->isDir())
			if (folder = container->open(files.at(i).second->fileName(), false, error))
				if (folder->contains(Plugin::fileName()))
					QMessageBox::warning(
							Application::mainWindow(),
							tr("Create database failed!"),
							tr("Error: Database already exist in\n"
							   "\"%1\"").
								arg(folder->location()));
				else
				{
					IdmStorage storage(folder->location(Plugin::fileName()), true);

					if (storage.isValid())
						QMessageBox::information(
								Application::mainWindow(),
								tr("Create database done!"),
								tr("All done"));
					else
						QMessageBox::critical(
								Application::mainWindow(),
								tr("Create database failed!"),
								tr("Error: ").
									append(storage.lastError()));
				}
			else
				QMessageBox::critical(
						Application::mainWindow(),
						tr("Create database failed!"),
						tr("Error: ").
							append(error));
}

IDM_PLUGIN_NS_END
