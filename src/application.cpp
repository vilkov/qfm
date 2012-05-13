#include "application.h"
#include "version.h"
#if defined(Q_WS_WIN)
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QThread>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtGui/QKeyEvent>

#include "filesystemplugins/default/defaultplugin.h"
#include "filesystemplugins/m3uplugin/m3uplugin.h"
#include "filesystemplugins/idmplugin/idmplugin.h"
#include "filesystemplugins/arcplugin/arcplugin.h"


FILESYSTEM_PLUGINS_NS_BEGIN

class AppRootNode : public RootNode
{
public:
	AppRootNode() :
		::FileSystem::RootNode()
	{
		registerStatic(&m_m3uplugin);
		registerStatic(&m_idmplugin);
		registerStatic(&m_arcplugin);

		/* XXX: Must be last in the list. */
		registerStatic(static_cast<IFilePlugin *>(&m_default));
		registerStatic(static_cast<IContainerPlugin *>(&m_default));
	}

private:
	M3u::Plugin m_m3uplugin;
	Idm::Plugin m_idmplugin;
	Arc::Plugin m_arcplugin;

private:
	Default::Plugin m_default;
};

static AppRootNode *appRootNode = NULL;

FILESYSTEM_PLUGINS_NS_END


Application::Application(const QString &name, const QString &organization, const QString &description, int &argc, char **argv, bool GUIenabled) :
	QApplication(argc, argv, GUIenabled),
	m_taskPool(QThread::idealThreadCount() * 2)
{
	QApplication::setApplicationName(name);
	QApplication::setOrganizationName(organization);
	QApplication::setApplicationVersion(version());
}

Application::~Application()
{
	m_taskPool.clear();
	delete ::FileSystem::Plugins::appRootNode;
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

::FileSystem::RootNode *Application::rootNode()
{
	if (::FileSystem::Plugins::appRootNode)
		return ::FileSystem::Plugins::appRootNode;
	else
		return ::FileSystem::Plugins::appRootNode = new ::FileSystem::Plugins::AppRootNode();
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
