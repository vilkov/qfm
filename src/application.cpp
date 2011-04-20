#include "application.h"
#include "version.h"
#include <QtGui/QKeyEvent>
#include <QtCore/QThread>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>


Application::Application(const QString &name, const QString &label, const QString &description, int &argc, char **argv, bool GUIenabled) :
	QApplication(argc, argv, GUIenabled),
	m_taskPool(QThread::idealThreadCount())
{
	QApplication::setApplicationName(label);
	QApplication::setApplicationVersion(version());
}

Application::~Application()
{
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
//		if (event->type() == QEvent::KeyRelease &&
//			static_cast<QKeyEvent*>(event)->key() == Qt::Key_Tab &&
//			static_cast<QKeyEvent*>(event)->modifiers() == Qt::NoModifier)
//			if (m_mainWindow.switchToOtherPanel(receiver))
//			{
//				event->accept();
//				return true;
//			}

			return QApplication::notify(receiver, event);
	}
	CATCH_ALL
	(
		QString::fromLatin1("Application::notify trows an unhandled exception: receiver \"%1\" event \"%2\"!").arg(receiver->objectName()).arg(QString::number(event->type())),
		return true;
	)
}

qint32 Application::exec()
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

void Application::handleException(const char *where)
{
//	m_log << QString::fromLatin1(where).append(QString::fromLatin1(" trows an unhandled exception!"));
}

void Application::handleException(const QString &message)
{
//	m_log << message;
}