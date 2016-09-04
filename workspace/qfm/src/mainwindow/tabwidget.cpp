/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "tabwidget.h"

#include <lvfs/Module>
#include <lvfs/IEntry>
#include <lvfs-core/INode>
#include <lvfs-core/IView>
#include <lvfs-core/IViewFactory>
#include <lvfs-core/models/Qt/Node>

#include <brolly/assert.h>
#include <efc/ScopedPointer>


namespace {
    using namespace LVFS;

    bool compare_tabs(Settings::Option *o1, Settings::Option *o2)
    {
        return static_cast<Settings::IntOption *>(*static_cast<Settings::Scope *>(o1)->options().begin())->value() <
                static_cast<Settings::IntOption *>(*static_cast<Settings::Scope *>(o2)->options().begin())->value();
    }
}


TabWidget::TabWidget(LVFS::Settings::Scope *settings, LVFS::Interface::Holder &opposite, QWidget *parent) :
    QWidget(parent),
    m_tabIndex("Index", 0),
    m_tabPath("Path", ""),
    m_tab("Tab"),
    m_tabs(&m_tab),
    m_currentTab("CurrentTab", 0),
    m_opposite(opposite),
    m_doNotRefreshTab(true),
    m_layout(this),
    m_tabWidget(this)
{
    m_layout.setMargin(3);
    m_layout.setSpacing(1);
    m_layout.addWidget(&m_tabWidget);
    m_layout.setStretchFactor(&m_tabWidget, 1);

    connect(&m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshTab(int)));

    m_tab.manage(&m_tabIndex);
    m_tab.manage(&m_tabPath);
    settings->manage(&m_currentTab);
    settings->manage(&m_tabs);
}

void TabWidget::open()
{
    using namespace LVFS;
    Module::Error error;
    Interface::Holder node;
    Settings::Scope *tab;
    Settings::StringOption *tabPath;

    m_tabs.values().sort(compare_tabs);

    for (Settings::List::Container::iterator i = m_tabs.values().begin();
         i != m_tabs.values().end();
         i = m_tabs.values().erase(i))
    {
        tab = static_cast<Settings::Scope *>(*i);
        tabPath = static_cast<Settings::StringOption *>(*(++tab->options().begin()));

        node = Core::INode::open(tabPath->value(), error);

        if (LIKELY(node.isValid() == true))
            show(node);

        delete *i;
    }

    if (m_views.empty())
    {
        node = Core::INode::open("/", error);

        if (LIKELY(node.isValid() == true))
            show(node);
    }

    m_tabWidget.setCurrentIndex(m_currentTab.value());
}

void TabWidget::close()
{
    using namespace LVFS;
    Interface::Holder node;
    Container::iterator it;
    EFC::ScopedPointer<Settings::Scope> tab;
    EFC::ScopedPointer<Settings::IntOption> tabIndex;
    EFC::ScopedPointer<Settings::StringOption> tabPath;

    m_currentTab.setValue(m_tabWidget.currentIndex());

    for (Container::iterator it = m_views.begin(); it != m_views.end(); it = m_views.erase(it))
    {
        node = it->second->as<Core::IView>()->node();
        ASSERT(node.isValid());

        tab.reset(new Settings::Scope(m_tab.id(), true));

        if (UNLIKELY(tab.get() == NULL))
            return;

        tabIndex.reset(new Settings::IntOption(m_tabIndex.id(), m_tabWidget.indexOf(it->second->as<Core::IView>()->widget())));

        if (UNLIKELY(tabIndex.get() == NULL))
            return;
        else
            tab->manage(tabIndex.release());

        tabPath.reset(new Settings::StringOption(m_tabPath.id(), node->as<Core::INode>()->file()->as<IEntry>()->location()));

        if (UNLIKELY(tabPath.get() == NULL))
            return;
        else
            tab->manage(tabPath.release());

        node->as<Core::INode>()->closed(it->second);

        for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->decRef();

        node->as<Core::INode>()->clear();
        m_tabs.values().push_back(tab.release());
    }

    m_tabWidget.clear();
}

const LVFS::Interface::Holder &TabWidget::opposite(const LVFS::Interface::Holder &view) const
{
    return m_opposite;
}

void TabWidget::show(const LVFS::Interface::Holder &view, const LVFS::Interface::Holder &n)
{
    using namespace LVFS;
    Interface::Holder node(n);
    Core::IView *coreView = view->as<Core::IView>();
    Interface::Holder oldViewNode(coreView->node());

    if (coreView->isAbleToView(node))
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

        coreView->setNode(node);
        node->as<Core::INode>()->opened(view);
        node->as<Core::INode>()->refresh();

        if (m_tabWidget.currentIndex() != -1)
            m_tabWidget.setTabText(m_tabWidget.currentIndex(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
        else
        {
            m_tabWidget.addTab(coreView->widget(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
            m_views[coreView->widget()] = view;
        }
    }
    else
        if (Core::IViewFactory *factory = node->as<Core::INode>()->file()->as<Core::IViewFactory>())
        {
            Interface::Holder newView = factory->createView();

            if (LIKELY(newView.isValid()))
            {
                coreView = newView->as<Core::IView>();
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

                coreView->setNode(node);
                node->as<Core::INode>()->opened(newView);
                node->as<Core::INode>()->refresh();

                int index = m_tabWidget.currentIndex();

                if (index == -1)
                    m_tabWidget.addTab(coreView->widget(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
                else
                {
                    ASSERT(m_tabWidget.currentWidget() == view->as<Core::IView>()->widget());

                    m_tabWidget.removeTab(index);
                    m_views.erase(view->as<Core::IView>()->widget());
                    m_tabWidget.insertTab(index, coreView->widget(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
                    m_tabWidget.setCurrentIndex(index);
                }

                m_views[coreView->widget()] = newView;
            }
        }
}

void TabWidget::show(const Interface::Holder &node)
{
    using namespace LVFS;

    if (Core::IViewFactory *factory = node->as<Core::INode>()->file()->as<Core::IViewFactory>())
    {
        Interface::Holder newView = factory->createView();

        if (LIKELY(newView.isValid()))
        {
            Core::IView *coreView = newView->as<Core::IView>();
            coreView->setMainView(Interface::Holder::fromRawData(this));

            for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
                n->as<Core::INode>()->incRef();

            coreView->setNode(node);
            node->as<Core::INode>()->opened(newView);
            node->as<Core::INode>()->refresh();

            int index = m_tabWidget.currentIndex();

            if (index == -1)
                m_tabWidget.addTab(coreView->widget(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
            else
            {
                m_tabWidget.insertTab(index + 1, coreView->widget(), Core::Qt::Node::toUnicode(node->as<Core::INode>()->file()->as<IEntry>()->title()));
                m_tabWidget.setCurrentIndex(index + 1);
            }

            m_views[coreView->widget()] = newView;
        }
    }
}

void TabWidget::close(const Interface::Holder &view)
{
    using namespace LVFS;
    ASSERT(m_tabWidget.indexOf(view->as<Core::IView>()->widget()) != -1);

    if (m_tabWidget.count() > 1)
    {
        Interface::Holder node = view->as<Core::IView>()->node();
        m_tabWidget.removeTab(m_tabWidget.indexOf(view->as<Core::IView>()->widget()));

        if (node.isValid())
        {
            node->as<Core::INode>()->closed(view);

            for (Interface::Holder n = node; n.isValid(); n = n->as<Core::INode>()->parent())
                if (n->as<Core::INode>()->decRef() == 0)
                    n->as<Core::INode>()->clear();
        }

        m_views.erase(view->as<Core::IView>()->widget());
    }
}

void TabWidget::refresh()
{
    doRefresh(m_tabWidget.currentWidget());
}

//void TabWidget::openInNewTab(::VFS::INode *node, const QModelIndex &index, const QList<qint32> &geometry)
//{
//    EFC::ScopedPointer<DirectoryView> widget(new DirectoryView(node, index, geometry, this));
//    m_doNotRefreshTab = true;
//    m_tabWidget.setCurrentIndex(m_tabWidget.addTab(widget.data(), widget->title()));
//    widget.take()->setFocus();
//}

void TabWidget::closeCurrentTab()
{
//    if (m_tabWidget.count() > 1)
//    {
//        EFC::ScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
//        m_tabWidget.removeTab(m_tabWidget.currentIndex());
//        static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setFocus();
//    }
}

void TabWidget::setCurrentDirectory(const QString &filePath)
{
//    static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setCurrentDirectory(filePath);
}

void TabWidget::skipOneRefreshTab()
{
    m_doNotRefreshTab = true;
}

void TabWidget::refreshTab(int index)
{
    doRefresh(m_tabWidget.widget(index));
}

void TabWidget::doRefresh(QWidget *tab)
{
//    if (m_doNotRefreshTab)
//        m_doNotRefreshTab = false;
//    else
//        static_cast<DirectoryView*>(tab)->refresh();
}

void TabWidget::save()
{
//    m_settings.setActiveTab(m_tabWidget.currentIndex());
//
//    for (qint32 i = 0, size = m_tabWidget.count(); i < size; ++i)
//        m_settings.tabs().add(static_cast<DirectoryView *>(m_tabWidget.widget(i))->tab());
}

void TabWidget::load()
{
//    if (m_settings.tabs().isEmpty())
//    {
//        EFC::ScopedPointer<DirectoryView> widget(new DirectoryView(DirectoryView::defaultPath(), this));
//        m_tabWidget.addTab(widget.data(), widget->title());
//        widget.release();
//    }
//    else
//    {
//        Tab *tab;
//        EFC::ScopedPointer<DirectoryView> widget;
//
//        for (Tabs::const_iterator i = m_settings.tabs().begin(), end = m_settings.tabs().end(); i != end; ++i)
//        {
//            tab = (*i)->as<Tab>();
//            widget.reset(new DirectoryView(tab->path(), tab->sort().column(), tab->sort().order(), tab->geometry(), tab->currentFile(), this));
//            m_tabWidget.addTab(widget.data(), widget->title());
//            widget.release();
//        }
//
//        m_tabWidget.setCurrentIndex(m_settings.activeTab());
//        m_settings.tabs().clear();
//    }
}

//TabWidget::Sort::Sort(Option *parent) :
//    Scope(QString::fromLatin1("Sort"), parent),
//    m_column(QString::fromLatin1("Column"), this),
//    m_order(QString::fromLatin1("Order"), this)
//{
//    manage(&m_column);
//    manage(&m_order);
//}
//
//TabWidget::Sort::Sort(const DirectoryView::Tab::Sort &sort, Option *parent) :
//    Scope(QString::fromLatin1("Sort"), parent),
//    m_column(QString::fromLatin1("Column"), this),
//    m_order(QString::fromLatin1("Order"), this)
//{
//    manage(&m_column);
//    manage(&m_order);
//
//    m_column.setValue(QString::number(sort.column));
//    m_order.setValue(QString::number(sort.order));
//}
//
//TabWidget::Tab::Tab(Option *parent) :
//    Scope(QString::fromLatin1("Tab"), parent),
//    m_path(QString::fromLatin1("Path"), this),
//    m_sort(this),
//    m_geometry(QString::fromLatin1("Geometry"), this),
//    m_currentFile(QString::fromLatin1("CurrentFile"), this)
//{
//    manage(&m_path);
//    manage(&m_sort);
//    manage(&m_geometry);
//    manage(&m_currentFile);
//}
//
//TabWidget::Tab::Tab(const DirectoryView::Tab &tab, Option *parent) :
//    Scope(QString::fromLatin1("Tab"), parent),
//    m_path(QString::fromLatin1("Path"), this),
//    m_sort(tab.sort, this),
//    m_geometry(QString::fromLatin1("Geometry"), this),
//    m_currentFile(QString::fromLatin1("CurrentFile"), this)
//{
//    manage(&m_path);
//    manage(&m_sort);
//    manage(&m_geometry);
//    manage(&m_currentFile);
//
//    m_path.setValue(tab.path);
//    m_currentFile.setValue(tab.currentFile);
//
//    for (DirectoryView::Geometry::size_type i = 0, size = tab.geometry.size(); i < size; ++i)
//        m_geometry.add(QString::number(tab.geometry.at(i)));
//}
//
//DirectoryView::Geometry TabWidget::Tab::geometry() const
//{
//    DirectoryView::Geometry res;
//    res.reserve(m_geometry.size());
//
//    for (::Tools::Settings::OptionValueList::size_type i = 0, size = m_geometry.size(); i < size; ++i)
//        res.push_back(m_geometry.at(i).toInt());
//
//    return res;
//}
//
//TabWidget::Tabs::Tabs(Option *parent) :
//    OptionList(QString::fromLatin1("Tabs"), parent)
//{}
//
//void TabWidget::Tabs::add(const DirectoryView::Tab &tab)
//{
//    OptionList::add(new Tab(tab, this));
//}
//
//bool TabWidget::Tabs::isSubOptionName(const QStringRef &name) const
//{
//    static const QString optionName = QString::fromLatin1("Tab");
//    return name == optionName;
//}
//
//::Tools::Settings::Option *TabWidget::Tabs::create()
//{
//    return new Tab(this);
//}
//
//TabWidget::Settings::Settings(const QString &id, Scope *parent, TabWidget *object, Method save, Method load) :
//    Scope(id, parent),
//    m_activeTab(QString::fromLatin1("ActiveTab"), this),
//    m_tabs(this),
//    m_object(object),
//    m_save(save),
//    m_load(load)
//{
//    parent->manage(this);
//    manage(&m_activeTab);
//    manage(&m_tabs);
//}
//
//void TabWidget::Settings::save(QXmlStreamWriter &stream) const
//{
//    (m_object->*m_save)();
//    Scope::save(stream);
//}
//
//void TabWidget::Settings::load(QXmlStreamReader &stream)
//{
//    Scope::load(stream);
//    (m_object->*m_load)();
//}
//
//void TabWidget::Settings::loadDefault()
//{
//    Scope::loadDefault();
//    (m_object->*m_load)();
//}
