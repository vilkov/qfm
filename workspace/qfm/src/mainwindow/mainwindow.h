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

#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <lvfs-core/IMainView>


class MainWindow : public QMainWindow, public LVFS::Implements<LVFS::Core::IMainView>
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    virtual ~MainWindow();

    using QMainWindow::show;

    void open();
    void close();

    /* LVFS::Core::IMainView */
    virtual const LVFS::Interface::Holder &opposite(const LVFS::Interface::Holder &view) const;
    virtual void show(const LVFS::Interface::Holder &view, const LVFS::Interface::Holder &node);

    void switchToOtherPanel();

private Q_SLOTS:
    void setupGeometry();

private:
    QSplitter m_centralWidget;
    QWidget m_leftWidget;
    QVBoxLayout m_left;
    QWidget m_rightWidget;
    QVBoxLayout m_right;

private:
    LVFS::Interface::Holder m_view[2];
};


#endif /* MAINWINDOW_H_ */
