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
#include "application.h"
#include "version.h"

#if defined(Q_WS_WIN)
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QThread>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtGui/QKeyEvent>
#include <QtGui/QMessageBox>

#include "vfs-plugins/default/default_plugin.h"
#include "vfs-plugins/m3uplugin/m3uplugin.h"
#include "vfs-plugins/idmplugin/idm_plugin.h"
#include "vfs-plugins/arcplugin/arc_plugin.h"


VFS_PLUGINS_NS_BEGIN

class AppRootNode : public RootNode
{
public:
	AppRootNode(::Tools::Settings::Page *pluginsPage) :
		RootNode(),
		m_default(pluginsPage),
		m_m3uplugin(pluginsPage),
		m_idmplugin(pluginsPage),
		m_arcplugin1(pluginsPage),
		m_arcplugin2(pluginsPage)
	{
		registerStatic(&m_m3uplugin);
		registerStatic(&m_idmplugin);
		registerStatic(&m_arcplugin1);
		registerStatic(&m_arcplugin2);

		/* XXX: Must be last in the list. */
		registerStatic(static_cast<IFilePlugin *>(&m_default));
		registerStatic(static_cast<IContainerPlugin *>(&m_default));

		/* Register settings pages. */
		pluginsPage->manage(m_default.settings());
		pluginsPage->manage(m_arcplugin2.settings());
		pluginsPage->manage(m_idmplugin.settings());
		pluginsPage->manage(m_m3uplugin.settings());
	}

private:
	Default::Plugin m_default;

private:
	M3u::Plugin m_m3uplugin;
	Idm::Plugin m_idmplugin;
	Arc::LibArchive::Plugin m_arcplugin1;
	Arc::LibUnrar::Plugin m_arcplugin2;
};

VFS_PLUGINS_NS_END


Application::Application(const QString &name, const QString &organization, const QString &description, int &argc, char **argv, bool GUIenabled) :
	QApplication(argc, argv, GUIenabled),
	m_taskPool(QThread::idealThreadCount() * 2),
	m_rootNode(new ::VFS::Plugins::AppRootNode(&m_settings2.pluginsPage()))
{
	QApplication::setApplicationName(name);
	QApplication::setOrganizationName(organization);
	QApplication::setApplicationVersion(version());

	m_settings2.load();

	setFont(m_settings2.generalPage().baseFont());
}

Application::~Application()
{
	m_settings2.save();
	m_taskPool.clear();
}

QString Application::version() const
{
	if (RELEASE_VERSION_NUMBER % 2 == 0)
		return QString::fromLatin1(VERSION_STRING);
	else
		return QString::fromLatin1(VERSION_STRING).append(QString::fromLatin1(" (unstable)"));
}

bool Application::notify(QObject *receiver, QEvent *event)
{
	TRY
	{
		if (event->type() == QEvent::KeyPress && //QEvent::KeyRelease &&
			(static_cast<QKeyEvent*>(event)->key() + static_cast<QKeyEvent*>(event)->modifiers()) == Qt::NoModifier + Qt::Key_Tab)
		{
			m_mainWindow.switchToOtherPanel();
			event->accept();
			return true;
		}

		return QApplication::notify(receiver, event);
	}
	CATCH_ALL
	(
		QString::fromLatin1("Application::notify trows an unhandled exception: receiver \"%1\" event \"%2\"!").
		arg(receiver->objectName()).
		arg(QString::number(event->type())),
		DO_NOTHING
	)

	return true;
}

qint32 Application::exec()
{
	if (m_settings.storageLocation().isEmpty())
		return 1;
	else
	{
		QTranslator qtTranslator;
		qtTranslator.load(QString::fromLatin1("qt_").append(QLocale::system().name()), QApplication::applicationDirPath());
		installTranslator(&qtTranslator);

		QTranslator appTranslator;
		qtTranslator.load(QString::fromLatin1("app_").append(QLocale::system().name()), QApplication::applicationDirPath());
		installTranslator(&appTranslator);

		m_mainWindow.show();
		return QApplication::exec();
	}
}

void Application::open(const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file)
{
	using namespace ::VFS;
	IApplications::LinkedList list = container->applications()->user(file->fileType());

	if (!list.isEmpty() || !(list = container->applications()->system(file->fileType())).isEmpty())
	{
		QString error;

		if (!list.front()->exec(container, file, error))
			QMessageBox::critical(Application::mainWindow(), tr("Failed to execute application..."), error);
	}
	else
		QMessageBox::critical(Application::mainWindow(),
				tr("Failed to execute application..."),
				tr("No applications able to handle \"%1\" files.").
					arg(file->fileType()->name()));
}

void Application::open(const ::VFS::IApplication *application, const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file)
{
	using namespace ::VFS;
	QString error;

	if (!application->exec(container, file, error))
		QMessageBox::critical(Application::mainWindow(), tr("Failed to execute application..."), error);
}

void Application::handleException(const char *where)
{
//	m_log << QString::fromLatin1(where).append(QString::fromLatin1(" trows an unhandled exception!"));
}

void Application::handleException(const QString &message)
{
//	m_log << message;
}

void Application::handleException(const char *where, const char *what)
{

}
