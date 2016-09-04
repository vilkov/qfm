/**
 * This file is part of qfm.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * qfm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qfm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qfm. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <lvfs/Interface>
#include <lvfs/settings/Scope>
#include <lvfs/settings/IntOption>
#include <QtGui/QSplitter>
#include <QtGui/QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(LVFS::Settings::Scope *settings);
    virtual ~MainWindow();

    using QMainWindow::show;

    void open();
    void close();

    void switchToOtherPanel();

private:
    LVFS::Settings::Scope m_settings;
    LVFS::Settings::Scope m_geometry;
    LVFS::Settings::IntOption m_geometryVal[4];
    LVFS::Settings::Scope m_tabsSettings[2];

private:
    QSplitter m_centralWidget;
    LVFS::Interface::Holder m_tabs[2];
};


#endif /* MAINWINDOW_H_ */
