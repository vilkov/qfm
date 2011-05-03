#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include "foldersview/foldersview.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

	bool switchToOtherPanel(QObject *receiver);

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
    QWidget m_centralWidget;
    QVBoxLayout m_layout;
    QSplitter m_splitter;
    FoldersView m_leftFoldersView;
    FoldersView m_rightFoldersView;

    /* Actions */
    FileMenuActions m_fileMenuActions;
    ToolsMenuActions m_toolsMenuActions;
};

#endif // MAINWINDOW_H
