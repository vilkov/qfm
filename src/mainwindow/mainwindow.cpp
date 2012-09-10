#include "mainwindow.h"
#include "../desktop/devices/desktop_device_partition.h"
#include "../desktop/devices/drives/desktop_device_removabledrive.h"
#include "../desktop/devices/drives/desktop_device_opticaldrive.h"
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
	QString tab = QString::fromLatin1("    %1 (%2)");
	QString label = QString::fromLatin1("%1 (%2)");
	::Desktop::Devices *devices = ::Desktop::Devices::instance();

	for (::Desktop::Devices::const_iterator i = devices->begin(), end = devices->end(); i != end; ++i)
		if ((*i)->isDrive())
		{
			const ::Desktop::Drive::Container &partitions = (*i)->as< ::Desktop::Drive >()->partitions();

			menu.addAction((*i)->icon(), QString(label).arg((*i)->label()).arg(::VFS::Tools::humanReadableShortSize((*i)->as< ::Desktop::Drive >()->size())))->setData(qVariantFromValue(static_cast<void *>(*i)));

			for (::Desktop::Drive::Container::const_iterator i = partitions.begin(), end = partitions.end(); i != end; ++i)
				menu.addAction((*i)->icon(), QString(tab).arg((*i)->label()).arg(::VFS::Tools::humanReadableShortSize((*i)->size())))->setData(qVariantFromValue(static_cast<void *>(*i)));
		}
		else
			menu.addAction((*i)->icon(), (*i)->label())->setData(qVariantFromValue(static_cast<void *>(*i)));

	if (QAction *res = menu.exec(view.listPos()))
	{
		::Desktop::Device *device = static_cast< ::Desktop::Device *>(res->data().value<void *>());

		if (device->isPartition())
		{
			QString error;
			::Desktop::Partition *partition = device->as< ::Desktop::Partition >();

			if (partition->mountPaths().isEmpty())
				if (partition->mount(error))
					view.setCurrentDirectory(partition->mountPaths().at(0));
				else
					QMessageBox::critical(this, tr("Error"), error);
			else
				view.setCurrentDirectory(partition->mountPaths().at(0));
		}
		else
			if (device->isRemovableDrive())
			{
				QString error;

				if (device->as< ::Desktop::RemovableDrive >()->isDetachable())
					if (!device->as< ::Desktop::RemovableDrive >()->eject(error))
						QMessageBox::critical(this, tr("Error"), error);
			}

		view.setFocus();
	}
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
