/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QByteArray>
#include <QtGui/QMainWindow>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include <tools/settings/options/settingswidgetscope.h>
#include "foldersview/foldersview.h"

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
