/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "idm_createdbaction.h"
#include "../idm_plugin.h"
#include "../containeres/idm_container.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

CreateDbAction::CreateDbAction() :
    SyncAction(QIcon(), tr("Create database..."))
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
//                                Storage storage(folder->location(Plugin::fileName()), folder->location(oldName));
//
//                                if (storage.isValid())
//                                    QMessageBox::information(
//                                            Application::mainWindow(),
//                                            tr("Create database"),
//                                            tr("All done"));
//                                else
//                                    QMessageBox::critical(
//                                            Application::mainWindow(),
//                                            tr("Create database"),
//                                            tr("Error: ").
//                                                append(QString::fromAscii("<FIXME>")));
                            }
                            else
                            {
                                Storage storage(fromUnicode(folder->location(Plugin::fileName())).data(), true);

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
                                                append(QString::fromAscii("<FIXME>")));
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
                    Storage storage(folder->location(Plugin::fileName()).as<QByteArray>().data(), true);

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
                                    append(QString::fromAscii("<FIXME>")));
                }
            else
                QMessageBox::critical(
                        Application::mainWindow(),
                        tr("Create database"),
                        tr("Error: ").
                            append(error));
}

IDM_PLUGIN_NS_END
