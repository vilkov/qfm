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
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QtCore/QThread>
#include <QtGui/QApplication>
//#include <QtSingleInstance/QSingleApplication>

#include <vfs/root/vfs_rootnode.h>
#include <vfs/actions/vfs_actions.h>
#include <tools/exceptionshandler/exceptionshandler.h>
#include <tools/taskspool/taskpool.h>

#include "settings/applicationsettings.h"
#include "settings/applicationsettingscontainer.h"
#include "mainwindow/mainwindow.h"
#include "desktop/locale/desktop_locale.h"
#include "desktop/theme/desktop_theme.h"
#include "desktop/devices/desktop_devices.h"


class Application : public QApplication, protected ExceptionHandler
{
	Q_OBJECT

public:
	Application(const QString &name, const QString &organization, const QString &description, int &argc, char **argv, bool GUIenabled = true);
	virtual ~Application();

	virtual bool notify(QObject *receiver, QEvent *event);

	qint32 exec();
	QString version() const;

	static Application *instance() { return static_cast<Application*>(QApplication::instance()); }
	static SettingsContainer *settings2() { return &instance()->m_settings2; };
	static ::VFS::Actions *globalActions() { return &instance()->m_globalActions; }
	static ::Tools::TasksPool::TaskPool *taskPool() { return &instance()->m_taskPool; }
	static ApplicationSettings *config() { return &instance()->m_settings; }
	static MainWindow *mainWindow() { return &instance()->m_mainWindow; }
	static ::VFS::RootNode *rootNode() { return instance()->m_rootNode.data(); }

	static void open(const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file);
	static void open(const ::VFS::IApplication *application, const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file);

protected: /* ExceptionHandler */
	virtual void handleException(const char *where);
	virtual void handleException(const QString &message);
	virtual void handleException(const char *where, const char *what);

private:
	::Desktop::Locale m_locale;
	::Desktop::Theme m_theme;
	::Desktop::Devices m_devices;
	SettingsContainer m_settings2;

	ApplicationSettings m_settings;
	::VFS::Actions m_globalActions;
	::Tools::TasksPool::TaskPool m_taskPool;
    ::Tools::Memory::ScopedPointer< ::VFS::RootNode > m_rootNode;
	MainWindow m_mainWindow;
};

#endif /* APPLICATION_H_ */
