#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QByteArray>
#include <QtGui/QMainWindow>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include "foldersview/foldersview.h"
#include "../de/mountpoints/mountpoints.h"
#include "../tools/settings/options/settingswidgetscope.h"

#include "view/dolphinitemlistcontainer.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

	void switchToOtherPanel();

protected:
    virtual void changeEvent(QEvent *event);

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
	void save();
	void load();

private:
    class Settings : public Tools::Settings::WidgetScope
    {
    public:
    	typedef void (MainWindow::*Method)();

    public:
    	Settings(MainWindow *object, Method save, Method load);

    protected:
    	virtual void save(QXmlStreamWriter &stream) const;
    	virtual void load(QXmlStreamReader &stream);
        virtual void loadDefault();

    private:
    	MainWindow *m_object;
    	Method m_save;
    	Method m_load;
    };

private:
    Settings m_settings;
    MountPoints m_mounts;

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
