#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include "foldersview/foldersview.h"
#include "../de/mountpoints/mountpoints.h"
#include "../filesystem/root/filesystemrootnode.h"

#include "view/dolphinitemlistcontainer.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

	void switchToOtherPanel();

protected:
    virtual void changeEvent(QEvent *event);

private:
    void saveTabs() const;
    void saveTabs(const FoldersView &panel, const QString &fileName) const;
    FoldersView::TabList loadLeftPanelTabs() const;
    FoldersView::TabList loadRightPanelTabs() const;
    FoldersView::TabList loadPanelTabs(const QString &fileName) const;


private:
    struct FileMenuActions
    {
    	FileMenuActions(QMainWindow *parent);

        QAction open;
        QAction exit;
    };
private Q_SLOTS:
    void actFileMenuOpen();
    void actFileMenuExit();


private:
    struct ToolsMenuActions
    {
    	ToolsMenuActions(QMainWindow *parent);

        QAction preferences;
    };
private Q_SLOTS:
    void actToolsMenuPreferences();


private:
    struct HelpMenuActions
    {
    	HelpMenuActions(QMainWindow *parent);

        QAction hotkeys;
    };
private Q_SLOTS:
    void actHelpMenuHotkeys();


private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QWidget
				>
			> CentralWidget;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					MainWindow
				>
			> CentralWidgetEventHandler;

	void showMountsForLeft();
	void showMountsForRight();
	void showMounts(FoldersView &view);

private:
    MountPoints m_mounts;
    FileSystem::RootNode m_root;

    CentralWidgetEventHandler m_eventHandler;
    CentralWidget m_centralWidget;
    QVBoxLayout m_layout;
    QSplitter m_splitter;
    FoldersView m_leftFoldersView;
    FoldersView m_rightFoldersView;

    /* Actions */
    FileMenuActions m_fileMenuActions;
    ToolsMenuActions m_toolsMenuActions;
    HelpMenuActions m_helpMenuActions;

    DolphinItemListContainer m_test;
};

#endif // MAINWINDOW_H
