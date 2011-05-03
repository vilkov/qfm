#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QtCore/QThread>
#include <QtGui/QApplication>
#include <QtGui/QFileIconProvider>
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
	virtual ~Application();

	virtual bool notify(QObject *receiver, QEvent *event);

	QFileIconProvider &iconProvider() { return m_iconProvider; }
	TasksPool::TaskPool &taskPool() { return m_taskPool; }
	ApplicationSettings &config() { return m_settings; }
	MainWindow &mainWindow() { return m_mainWindow; }

	qint32 exec();
	QString version() const;

	static Application *instance() { return static_cast<Application*>(QApplication::instance()); }

//#if defined(Q_WS_WIN)
//	typedef bool (*WinEventFilter)(MSG *message, long *result);
//	WinEventFilter setWinEventFilter(WinEventFilter filter) { WinEventFilter old = m_filter; m_filter = filter; return old; }
//    virtual bool winEventFilter(MSG *message, long *result);
//#endif

protected: /* ExceptionHandler */
	virtual void handleException(const char *where);
	virtual void handleException(const QString &message);

private:
	QFileIconProvider m_iconProvider;
	ApplicationSettings m_settings;
	TasksPool::TaskPool m_taskPool;
	MainWindow m_mainWindow;
//#if defined(Q_WS_WIN)
//	WinEventFilter m_filter;
//#endif
};

#endif /* APPLICATION_H_ */
