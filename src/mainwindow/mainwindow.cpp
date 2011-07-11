#include "mainwindow.h"
#include "../filesystem/tools/filesystemcommontools.h"
#include "../application.h"
#include <QtCore/QDir>
#include <QtCore/QTextCodec>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtXml/QXmlStreamWriter>

#include <QtGui/QMessageBox>
#include <QtGui/QCursor>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    m_eventHandler(this),
    m_centralWidget(&m_eventHandler, this),
    m_layout(&m_centralWidget),
    m_splitter(&m_centralWidget),
    m_leftFoldersView(&m_root, loadLeftPanelTabs(), m_rightFoldersView, &m_splitter),
    m_rightFoldersView(&m_root, loadRightPanelTabs(), m_leftFoldersView, &m_splitter),

    /* Actions */
    m_fileMenuActions(this),
    m_toolsMenuActions(this)
{
	setCentralWidget(&m_centralWidget);
	m_centralWidget.setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.setSpacing(1);

	m_layout.addWidget(&m_splitter);
	m_splitter.setChildrenCollapsible(false);
	m_splitter.addWidget(&m_leftFoldersView);
	m_splitter.addWidget(&m_rightFoldersView);

	Application::instance()->config().loadState(this);
	m_leftFoldersView.setFocus();

	m_eventHandler.registerShortcut(Qt::ALT, Qt::Key_F1, &MainWindow::showMountsForLeft);
	m_eventHandler.registerShortcut(Qt::ALT, Qt::Key_F2, &MainWindow::showMountsForRight);
}

MainWindow::~MainWindow()
{
	Application::instance()->config().saveState(this);
	saveTabs();
}

bool MainWindow::switchToOtherPanel(QObject *receiver)
{
	do
		if (receiver == &m_leftFoldersView)
		{
			m_leftFoldersView.setFocus();
			return true;
		}
		else
			if (receiver == &m_rightFoldersView)
			{
				m_rightFoldersView.setFocus();
				return true;
			}
			else
				receiver = receiver->parent();
	while (receiver);

	return false;
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

void MainWindow::saveTabs() const
{
	QDir dir(ApplicationSettings::storageLocation());

	saveTabs(m_leftFoldersView, dir.absoluteFilePath(QString::fromLatin1("leftpaneltabs.xml")));
	saveTabs(m_rightFoldersView, dir.absoluteFilePath(QString::fromLatin1("rightpaneltabs.xml")));
}

void MainWindow::saveTabs(const FoldersView &panel, const QString &fileName) const
{
	QFile file(fileName);

	if (file.open(QFile::WriteOnly | QFile::Truncate))
	{
		QXmlStreamWriter stream(&file);
		stream.setCodec(QTextCodec::codecForName("UTF-8"));
		stream.setAutoFormatting(true);

		stream.writeStartDocument(QString::fromLatin1("1.0"));

		stream.writeStartElement(QString::fromLatin1("Tabs"));
		panel.saveTabs(stream);
		stream.writeEndElement();

		stream.writeEndDocument();
	}
}

FoldersView::TabList MainWindow::loadLeftPanelTabs() const
{
	QDir dir(ApplicationSettings::storageLocation());
	return loadPanelTabs(dir.absoluteFilePath(QString::fromLatin1("leftpaneltabs.xml")));
}

FoldersView::TabList MainWindow::loadRightPanelTabs() const
{
	QDir dir(ApplicationSettings::storageLocation());
	return loadPanelTabs(dir.absoluteFilePath(QString::fromLatin1("rightpaneltabs.xml")));
}

FoldersView::TabList MainWindow::loadPanelTabs(const QString &fileName) const
{
	QFile file(fileName);

	if (file.open(QFile::ReadOnly))
	{
		QXmlStreamReader stream(&file);

		if (!stream.atEnd() && stream.readNext() == QXmlStreamReader::StartDocument)
			if (stream.readNextStartElement() && stream.name() == QLatin1String("Tabs"))
				return FoldersView::loadTabs(stream);
	}

	return FoldersView::TabList();
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

	m_mounts.refresh();
	for (MountPoints::size_type i = 0, size = m_mounts.size(); i < size; ++i)
	{
		const MountPoints::MountPoint &mount = m_mounts.at(i);

		if (mount.totalSize() > 0 && mount.freeSize() <= mount.totalSize())
			menu.addAction(mount.icon(), QString(mount.label()).append(QString(label).arg(FileSystem::Tools::humanReadableShortSize(mount.freeSize())).arg(FileSystem::Tools::humanReadableShortSize(mount.totalSize()))))->setData(i);
		else
			menu.addAction(mount.icon(), mount.label())->setData(i);
	}

	if (QAction *res = menu.exec(QCursor::pos()))
	{
		view.setCurrentDirectory(m_mounts.at(res->data().toInt()).path());
		view.setFocus();
	}
}
