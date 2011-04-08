#include "mainwindow.h"
#include "../application.h"
#include <QMenuBar>
#include <QMenu>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    m_centralWidget(this),
    m_layout(&m_centralWidget),
    m_splitter(&m_centralWidget),
#ifdef Q_OS_WIN32
    m_leftFoldersView(QStringList() << QString::fromLatin1("C:/"), &m_splitter),
    m_rightFoldersView(QStringList() << QString::fromLatin1("C:/"), &m_splitter),
#else
    m_leftFoldersView(QStringList() << QString::fromLatin1("/home"), &m_splitter),
    m_rightFoldersView(QStringList() << QString::fromLatin1("/"), &m_splitter),
#endif
    /* Actions */
    m_fileMenuActions(this),
    m_toolsMenuActions(this)
{
	setCentralWidget(&m_centralWidget);
	m_centralWidget.setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.setSpacing(1);

	m_layout.addWidget(&m_splitter);
	m_splitter.addWidget(&m_leftFoldersView);
	m_splitter.addWidget(&m_rightFoldersView);

	m_leftFoldersView.setFocus();
	Application::instance()->config().loadState(this);
}

MainWindow::~MainWindow()
{
	Application::instance()->config().saveState(this);
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
