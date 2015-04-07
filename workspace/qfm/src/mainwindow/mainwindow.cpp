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

#include <lvfs/Module>
#include <lvfs-core/INode>
#include <lvfs-core/IView>
#include <lvfs-core/IViewFactory>

#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setCentralWidget(&m_centralWidget);

    m_left.setMargin(0);
    m_right.setMargin(0);
    m_leftWidget.setLayout(&m_left);
    m_rightWidget.setLayout(&m_right);
    m_centralWidget.addWidget(&m_leftWidget);
    m_centralWidget.addWidget(&m_rightWidget);

    setupGeometry();
}

MainWindow::~MainWindow()
{}

void MainWindow::open()
{
    using namespace LVFS;

    Module::Error error;
    Interface::Holder node = Core::INode::open("file:///media/STORAGE/STORAGE", error);

    if (LIKELY(node.isValid() == true))
    {
        m_view[0] = node->as<Core::INode>()->file()->as<Core::IViewFactory>()->createView();
        m_view[0]->as<Core::IView>()->setMainView(Interface::Holder::fromRawData(this));
        m_left.addWidget(m_view[0]->as<Core::IView>()->widget());
        show(m_view[0], node);
    }

    node = Core::INode::open("file:///media/STORAGE/STORAGE/DB", error);

    if (LIKELY(node.isValid() == true))
    {
        m_view[1] = node->as<Core::INode>()->file()->as<Core::IViewFactory>()->createView();
        m_view[1]->as<Core::IView>()->setMainView(Interface::Holder::fromRawData(this));
        m_right.addWidget(m_view[1]->as<Core::IView>()->widget());
        show(m_view[1], node);
    }
}

void MainWindow::close()
{
    using namespace LVFS;
    Interface::Holder node = m_view[0]->as<Core::IView>()->node();

    if (node.isValid())
    {
        node->as<Core::INode>()->closed(m_view[0]);

        for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->decRef();

        node->as<Core::INode>()->clear();
    }

    node = m_view[1]->as<Core::IView>()->node();

    if (node.isValid())
    {
        node->as<Core::INode>()->closed(m_view[1]);

        for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->decRef();

        node->as<Core::INode>()->clear();
    }

    m_view[0].reset();
    m_view[1].reset();
}

const LVFS::Interface::Holder &MainWindow::opposite(const LVFS::Interface::Holder &view) const
{
    using namespace LVFS;

    if (m_view[0] == view)
        return m_view[1];
    else
        return m_view[0];
}

void MainWindow::show(const LVFS::Interface::Holder &view, const LVFS::Interface::Holder &n)
{
    using namespace LVFS;
    Interface::Holder node(n);
    Core::IView *coreView = view->as<Core::IView>();
    Interface::Holder oldViewNode(coreView->node());

    if (coreView->setNode(node))
    {
        for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->incRef();

        if (oldViewNode.isValid())
        {
            oldViewNode->as<Core::INode>()->closed(view);

            for (Interface::Holder n = oldViewNode; n.isValid(); n = n->as<Core::INode>()->parent())
                if (n->as<Core::INode>()->decRef() == 0)
                    n->as<Core::INode>()->clear();
        }

        node->as<Core::INode>()->opened(view);
        node->as<Core::INode>()->refresh();
    }
    else
        if (Core::IViewFactory *factory = node->as<Core::INode>()->file()->as<Core::IViewFactory>())
        {
            Interface::Holder newView = factory->createView();

            if (LIKELY(newView.isValid()))
            {
                coreView = newView->as<Core::IView>();

                if (coreView->setNode(node))
                {
                    coreView->setMainView(Interface::Holder::fromRawData(this));

                    for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
                        n->as<Core::INode>()->incRef();

                    if (oldViewNode.isValid())
                    {
                        oldViewNode->as<Core::INode>()->closed(view);

                        for (Interface::Holder n = oldViewNode; n.isValid(); n = n->as<Core::INode>()->parent())
                            if (n->as<Core::INode>()->decRef() == 0)
                                n->as<Core::INode>()->clear();
                    }

                    node->as<Core::INode>()->opened(newView);
                    node->as<Core::INode>()->refresh();

                    if (m_view[0] == view)
                    {
                        m_view[0] = newView;
                        m_left.removeWidget(view->as<Core::IView>()->widget());
                        m_left.addWidget(newView->as<Core::IView>()->widget());
                    }
                    else
                    {
                        m_view[1] = newView;
                        m_right.removeWidget(view->as<Core::IView>()->widget());
                        m_right.addWidget(newView->as<Core::IView>()->widget());
                    }
                }
            }
        }
}

void MainWindow::switchToOtherPanel()
{
    using namespace LVFS;
    Core::IView *view[2] = { m_view[0]->as<Core::IView>(), m_view[1]->as<Core::IView>() };

    if (view[0]->widget()->hasFocus())
        view[1]->widget()->setFocus();
    else
        view[0]->widget()->setFocus();
}

void MainWindow::setupGeometry()
{
    QRect rect = geometry();

    rect.setWidth(1024);
    rect.setHeight(768);
    rect.moveCenter(QApplication::desktop()->availableGeometry().center());

    setGeometry(rect);
}
