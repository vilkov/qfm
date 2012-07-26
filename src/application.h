#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QtCore/QThread>
#include <QtGui/QApplication>
//#include <QtSingleInstance/QSingleApplication>

#include <vfs/root/vfs_rootnode.h>

#include "tools/exceptionshandler/exceptionshandler.h"
#include "tools/taskspool/taskpool.h"
#include "settings/applicationsettings.h"
#include "settings/applicationsettingscontainer.h"
#include "mainwindow/mainwindow.h"
#include "de/de_locale.h"
#include "de/de_service.h"
#include "de/contextmenu/decontextmenufactory.h"


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
	static DesktopEnvironment::Locale *locale() { return &instance()->m_desktopLocale; }
	static DesktopEnvironment::Service *desktopService() { return &instance()->m_desktopService; }
    static DesktopEnvironment::ContextMenuFactory *globalMenu() { return &instance()->m_globalMenu; }
	static Tools::TasksPool::TaskPool *taskPool() { return &instance()->m_taskPool; }
	static ApplicationSettings *config() { return &instance()->m_settings; }
	static MainWindow *mainWindow() { return &instance()->m_mainWindow; }
	static ::VFS::RootNode *rootNode() { return instance()->m_rootNode.data(); }

protected: /* ExceptionHandler */
	virtual void handleException(const char *where);
	virtual void handleException(const QString &message);
	virtual void handleException(const char *where, const char *what);

private:
	SettingsContainer m_settings2;

	ApplicationSettings m_settings;
	Tools::TasksPool::TaskPool m_taskPool;
	DesktopEnvironment::Locale m_desktopLocale;
	DesktopEnvironment::Service m_desktopService;
    DesktopEnvironment::ContextMenuFactory m_globalMenu;
    QScopedPointer<VFS::RootNode> m_rootNode;
	MainWindow m_mainWindow;
};

#endif /* APPLICATION_H_ */
