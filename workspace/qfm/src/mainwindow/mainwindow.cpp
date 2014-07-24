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

#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

#include <lvfs/Module>
#include <lvfs-core/INode>
#include <lvfs-core/IView>
#include <lvfs-core/IViewFactory>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setCentralWidget(&m_centralWidget);
    setupGeometry();
}

MainWindow::~MainWindow()
{}

void MainWindow::open()
{
    using namespace LVFS;

    Module::Error error;
    Interface::Holder node = Core::INode::open("/home/dav", error);

    if (LIKELY(node.isValid() == true))
    {
        Interface::Holder parent;
        Interface::Holder currentNode = node;

        m_view[0] = Core::INode::view(node);
        m_view[1] = Core::INode::view(node);

        do
        {
            parent = currentNode->as<Core::INode>()->parent();
            currentNode->as<Core::INode>()->opened(m_view[0]);
            currentNode->as<Core::INode>()->opened(m_view[1]);
            currentNode = parent;
        }
        while (currentNode.isValid());

        m_view[0]->as<Core::IView>()->setMainView(Interface::Holder::fromRawData(this));
        m_centralWidget.addWidget(m_view[0]->as<Core::IView>()->widget());
        show(m_view[0], node);

        m_view[1]->as<Core::IView>()->setMainView(Interface::Holder::fromRawData(this));
        m_centralWidget.addWidget(m_view[1]->as<Core::IView>()->widget());
        show(m_view[1], node);
    }
}

void MainWindow::close()
{
    using namespace LVFS;

    Interface::Holder parent;
    Interface::Holder node = m_view[0]->as<Core::IView>()->node();

    if (node.isValid())
        do
        {
            parent = node->as<Core::INode>()->parent();
            node->as<Core::INode>()->closed(m_view[0]);
            node = parent;
        }
        while (node.isValid());

    m_view[0]->as<Core::IView>()->setNode(Interface::Holder());
    node = m_view[1]->as<Core::IView>()->node();

    if (node.isValid())
        do
        {
            parent = node->as<Core::INode>()->parent();
            node->as<Core::INode>()->closed(m_view[1]);
            node = parent;
        }
        while (node.isValid());

    m_view[1]->as<Core::IView>()->setNode(Interface::Holder());

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

void MainWindow::show(const LVFS::Interface::Holder &view, const LVFS::Interface::Holder &node)
{
    using namespace LVFS;
    Core::INode *coreNode = node->as<Core::INode>();
    Core::IView *coreView = view->as<Core::IView>();
    Interface::Holder oldViewNode(coreView->node());

    if (coreView->setNode(node))
    {
        if (oldViewNode.isValid())
        {
            Core::INode *oldViewCoreNode = oldViewNode->as<Core::INode>();

            if (oldViewCoreNode->parent() == node)
                oldViewCoreNode->closed(view);
            else
                node->as<Core::INode>()->opened(view);
        }

        node->as<Core::INode>()->refresh();
    }
    else if (Core::IViewFactory *factory = coreNode->file()->as<Core::IViewFactory>())
    {
        Interface::Holder newView = factory->createView();

        if (LIKELY(newView.isValid() == true))
        {
            coreView = newView->as<Core::IView>();

            if (coreView->setNode(node))
            {
                coreView->setMainView(Interface::Holder::fromRawData(this));

                if (oldViewNode.isValid())
                {
                    Core::INode *oldViewCoreNode = oldViewNode->as<Core::INode>();

                    if (oldViewCoreNode->parent() == node)
                        oldViewCoreNode->closed(view);
                    else
                        node->as<Core::INode>()->opened(newView);
                }

                node->as<Core::INode>()->refresh();

                m_view[0] == view ? m_view[0] = newView : m_view[1] = newView;
                m_centralWidget.addWidget(coreView->widget());
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
