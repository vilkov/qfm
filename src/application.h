#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QThread>
#include <QApplication>
//#include <QtSingleInstance/QSingleApplication>
#include "tools/exceptionshandler/exceptionshandler.h"
#include "tools/taskspool/taskpool.h"
#include "settings/applicationsettings.h"
#include "mainwindow/mainwindow.h"
#include "iconprovider/iconprovider.h"


class Application : public QApplication, protected ExceptionHandler
{
	Q_OBJECT

public:
	Application(const QString &name, const QString &label, const QString &description, int &argc, char **argv, bool GUIenabled = true);

	virtual bool notify(QObject *receiver, QEvent *event);

	TasksPool::TaskPool &taskPool() { return m_taskPool; }
	ApplicationSettings &config() { return m_settings; }
	IconProvider &iconProvider() { return m_iconProvider; }
	MainWindow &mainWindow() { return m_mainWindow; }

	qint32 exec();
	QString version() const;
	static Application *instance() { return static_cast<Application*>(QApplication::instance()); }

protected: /* ExceptionHandler */
	virtual void handleException(const char *where);
	virtual void handleException(const QString &message);

private:
	TasksPool::TaskPool m_taskPool;
	ApplicationSettings m_settings;
	IconProvider m_iconProvider;
	MainWindow m_mainWindow;
};

#endif /* APPLICATION_H_ */
