#include "mainwindow.h"
#include "../application.h"

#include <vfs/tools/vfs_commontools.h>

#include <QtCore/QDir>
#include <QtCore/QTextCodec>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtXml/QXmlStreamWriter>
#include <QtGui/QMessageBox>
#include <QtGui/QStyle>
#include <QtGui/QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    m_settings(this, &MainWindow::save, &MainWindow::load),
    m_eventHandler(this),
    m_centralWidget(&m_eventHandler, this),
    m_layout(&m_centralWidget),
    m_splitter(&m_centralWidget),
    m_leftFoldersView(QString::fromLatin1("LeftPanel"), &m_settings, m_rightFoldersView, &m_splitter),
    m_rightFoldersView(QString::fromLatin1("RightPanel"), &m_settings, m_leftFoldersView, &m_splitter),

    /* Actions */
    m_fileMenuActions(this),
    m_toolsMenuActions(this),
	m_helpMenuActions(this)
{
	QIcon icon;

	icon.addFile(QString::fromLatin1(":/icons/red-folder-16.png"), QSize(16, 16));
	icon.addFile(QString::fromLatin1(":/icons/red-folder-32.png"), QSize(32, 32));
	setWindowIcon(icon);

	setCentralWidget(&m_centralWidget);
	m_centralWidget.setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.setSpacing(1);

	m_layout.addWidget(&m_splitter);
	m_splitter.setChildrenCollapsible(false);
	m_splitter.addWidget(&m_leftFoldersView);
	m_splitter.addWidget(&m_rightFoldersView);

	m_eventHandler.registerShortcut(Qt::ALT, Qt::Key_F1, &MainWindow::showMountsForLeft);
	m_eventHandler.registerShortcut(Qt::ALT, Qt::Key_F2, &MainWindow::showMountsForRight);
}

void MainWindow::switchToOtherPanel()
{
	if (m_leftFoldersView.hasFocus())
		m_rightFoldersView.setFocus();
	else
		m_leftFoldersView.setFocus();
}

void MainWindow::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::ActivationChange && isActiveWindow())
	{
		m_leftFoldersView.refresh();
		m_rightFoldersView.refresh();
	}

	QMainWindow::changeEvent(event);
}

MainWindow::FileMenuActions::FileMenuActions(QMainWindow *parent) :
	open(tr("Open..."), parent),
	exit(tr("Exit"), parent)
{
	QMenu *menu = parent->menuBar()->addMenu(tr("File"));

	open.setShortcuts(QKeySequence::Open);
	open.setStatusTip(tr("Open an existing file"));
	parent->connect(&open, SIGNAL(triggered()), SLOT(actFileMenuOpen()));
	menu->addAction(&open);
	menu->addSeparator();

	exit.setShortcuts(QKeySequence::Quit);
	exit.setStatusTip(tr("Exit the application"));
	parent->connect(&exit, SIGNAL(triggered()), SLOT(actFileMenuExit()));
	menu->addAction(&exit);
}

void MainWindow::actFileMenuOpen()
{

}

void MainWindow::actFileMenuExit()
{
	Application::quit();
}

MainWindow::ToolsMenuActions::ToolsMenuActions(QMainWindow *parent) :
	preferences(tr("Preferences"), parent)
{
	QMenu *menu = parent->menuBar()->addMenu(tr("Tools"));

	preferences.setShortcuts(QKeySequence::Preferences);
	preferences.setStatusTip(tr("Show a preferences dialog"));
	parent->connect(&preferences, SIGNAL(triggered()), SLOT(actToolsMenuPreferences()));
	menu->addAction(&preferences);
}

void MainWindow::actToolsMenuPreferences()
{
	Application::settings2()->exec(this);
}

MainWindow::HelpMenuActions::HelpMenuActions(QMainWindow *parent) :
	hotkeys(tr("Hotkeys"), parent)
{
	QMenu *menu = parent->menuBar()->addMenu(tr("Help"));

	hotkeys.setShortcuts(QKeySequence::Preferences);
	hotkeys.setStatusTip(tr("Show a preferences dialog"));
	parent->connect(&hotkeys, SIGNAL(triggered()), SLOT(actHelpMenuHotkeys()));
	menu->addAction(&hotkeys);
}

void MainWindow::actHelpMenuHotkeys()
{
	static const char *help = "ALT+F1 \t\t- show mount points for the left panel.\n"
							  "ALT+F2 \t\t- show mount points for the right panel.\n"
							  "Backspace \t\t- go to parent directory.\n"
							  "ALT+Up \t\t- edit path of current tab.\n"
							  "CTRL+F5 \t\t- refresh.\n"
							  "ALT+CTRL+X \t\t- copy path of selected files to clipboard.\n"
							  "CTRL+Return \t\t- open selected folder in new tab.\n"
							  "CTRL+W/CTRL+Down \t- close current tab.\n"
							  "F2 \t\t- rename.\n"
							  "F7 \t\t- create directory.\n"
							  "SHIFT+Delete \t\t- remove selected files/folders.\n"
							  "Space \t\t- calculate folder size.\n"
							  "F5 \t\t- copy selected files/folders.\n"
							  "F6 \t\t- move selected files/folders.\n"
							  "Esc \t\t- cancel current operation (copy, move, delete, calculate size etc).";

	QMessageBox::information(this, tr("Hotkeys"), QString::fromLatin1(help));
}

void MainWindow::showMountsForLeft()
{
	showMounts(m_leftFoldersView);
}

void MainWindow::showMountsForRight()
{
	showMounts(m_rightFoldersView);
}

void MainWindow::showMounts(FoldersView &view)
{
	QMenu menu;
	QString label = QString::fromLatin1("   [free %1 of %2]");

//	m_mounts.refresh();
//	for (MountPoints::size_type i = 0, size = m_mounts.size(); i < size; ++i)
//	{
//		const MountPoint &mount = m_mounts.at(i);
//
//		if (mount.totalSize() > 0 && mount.freeSize() <= mount.totalSize())
//			menu.addAction(mount.icon(), QString(mount.label()).append(QString(label).arg(::VFS::Tools::humanReadableShortSize(mount.freeSize())).arg(::VFS::Tools::humanReadableShortSize(mount.totalSize()))))->setData(i);
//		else
//			menu.addAction(mount.icon(), mount.label())->setData(i);
//	}
//
//	if (QAction *res = menu.exec(view.listPos()))
//	{
//		view.setCurrentDirectory(m_mounts.at(res->data().toInt()).path());
//		view.setFocus();
//	}
}

void MainWindow::save()
{
	m_settings.setGeometry(saveGeometry());
}

void MainWindow::load()
{
	QByteArray geometry = m_settings.geometry();

	if (geometry.isEmpty())
		setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), Application::instance()->desktop()->availableGeometry()));
	else
		restoreGeometry(geometry);

	m_leftFoldersView.setFocus();
}

MainWindow::Settings::Settings(MainWindow *object, Method save, Method load) :
	WidgetScope(QString::fromLatin1("MainWindow"), 	Application::settings2()),
	m_object(object),
	m_save(save),
	m_load(load)
{
	Application::settings2()->manage(this);
}

void MainWindow::Settings::save(QXmlStreamWriter &stream) const
{
	(m_object->*m_save)();
	WidgetScope::save(stream);
}

void MainWindow::Settings::load(QXmlStreamReader &stream)
{
	WidgetScope::load(stream);
	(m_object->*m_load)();
}

void MainWindow::Settings::loadDefault()
{
	WidgetScope::loadDefault();
	(m_object->*m_load)();
}
