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

#include "mainwindow.h"
#include "tabwidget.h"

#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtCore/QByteArray>

#include <cstring>


MainWindow::MainWindow(LVFS::Settings::Scope *settings) :
    QMainWindow(),
    m_settings("MainWindow"),
    m_geometry("Geometry"),
    m_geometryVal({
                      { "left", -1 },
                      { "top", -1 },
                      { "width", -1 },
                      { "height", -1 }
                  }),
    m_tabsSettings({
                       { "LeftPanel" },
                       { "RightPanel" }
                   })
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setCentralWidget(&m_centralWidget);

    m_tabs[0].reset(new TabWidget(&m_tabsSettings[0], m_tabs[1]));
    m_tabs[1].reset(new TabWidget(&m_tabsSettings[1], m_tabs[0]));

    m_centralWidget.addWidget(m_tabs[0].as<TabWidget>());
    m_centralWidget.addWidget(m_tabs[1].as<TabWidget>());

    m_geometry.manage(&m_geometryVal[0]);
    m_geometry.manage(&m_geometryVal[1]);
    m_geometry.manage(&m_geometryVal[2]);
    m_geometry.manage(&m_geometryVal[3]);

    m_settings.manage(&m_geometry);
    m_settings.manage(&m_tabsSettings[0]);
    m_settings.manage(&m_tabsSettings[1]);
    settings->manage(&m_settings);
}

MainWindow::~MainWindow()
{
    m_tabs[0].reset();
    m_tabs[1].reset();
}

void MainWindow::open()
{
    if (m_geometryVal[0].value() != -1)
        setGeometry(QRect(m_geometryVal[0].value(), m_geometryVal[1].value(), m_geometryVal[2].value(), m_geometryVal[3].value()));
    else
    {
        QRect rect = geometry();

        rect.setWidth(1024);
        rect.setHeight(768);
        rect.moveCenter(QApplication::desktop()->availableGeometry().center());

        setGeometry(rect);
    }

    m_tabs[0].as<TabWidget>()->open();
    m_tabs[1].as<TabWidget>()->open();
}

void MainWindow::close()
{
    QRect rect = geometry();

    m_geometryVal[0].setValue(rect.left());
    m_geometryVal[1].setValue(rect.top());
    m_geometryVal[2].setValue(rect.width());
    m_geometryVal[3].setValue(rect.height());

    m_tabs[0].as<TabWidget>()->close();
    m_tabs[1].as<TabWidget>()->close();
}

void MainWindow::switchToOtherPanel()
{
    if (m_tabs[0].as<TabWidget>()->hasFocus())
        m_tabs[1].as<TabWidget>()->setFocus();
    else
        m_tabs[0].as<TabWidget>()->setFocus();
}
