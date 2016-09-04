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

#ifndef TABWIDGET_H_
#define TABWIDGET_H_

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>

#include <lvfs/settings/List>
#include <lvfs/settings/Scope>
#include <lvfs/settings/IntOption>
#include <lvfs/settings/StringOption>
#include <lvfs-core/IMainView>
#include <efc/Map>


class TabWidget : public QWidget, public LVFS::Implements<LVFS::Core::IMainView>
{
    Q_OBJECT

public:
    TabWidget(LVFS::Settings::Scope *settings, LVFS::Interface::Holder &opposite, QWidget *parent = 0);

    void open();
    void close();

    bool hasFocus() const { return m_tabWidget.currentWidget()->hasFocus(); }
    void setFocus() { m_tabWidget.currentWidget()->setFocus(); }

public: /* LVFS::Core::IMainView */
    virtual const LVFS::Interface::Holder &opposite(const LVFS::Interface::Holder &view) const;
    virtual void show(const LVFS::Interface::Holder &view, const LVFS::Interface::Holder &node);
    virtual void show(const Interface::Holder &node);
    virtual void close(const Interface::Holder &view);



public:
    void refresh();
//    void openInNewTab(::VFS::INode *node, const QModelIndex &index, const QList<qint32> &geometry);
    void closeCurrentTab();
    void setCurrentDirectory(const QString &filePath);

    void skipOneRefreshTab();

    QWidget *currentWidget() const { return m_tabWidget.currentWidget(); }

private Q_SLOTS:
    void refreshTab(int index);

private:
    void doRefresh(QWidget *tab);

private:
    void save();
    void load();

private:
//    class Sort : public LVFS::Settings::Scope
//    {
//    public:
//        Sort(Option *parent);
//        Sort(const DirectoryView::Tab::Sort &sort, Option *parent);
//
//        qint32 column() const { return m_column.value().toInt(); }
//        Qt::SortOrder order() const { return static_cast<Qt::SortOrder>(m_order.value().toInt()); }
//
//    private:
//        LVFS::Settings::OptionValue m_column;
//        LVFS::Settings::OptionValue m_order;
//    };
//
//
//    class Tab : public LVFS::Settings::Scope
//    {
//    public:
//        Tab(Option *parent);
//        Tab(const DirectoryView::Tab &tab, Option *parent);
//
//        QString path() const { return m_path.value(); }
//        const Sort &sort() const { return m_sort; }
//        DirectoryView::Geometry geometry() const;
//        const QString &currentFile() const { return m_currentFile.value(); }
//
//    private:
//        LVFS::Settings::OptionValue m_path;
//        Sort m_sort;
//        LVFS::Settings::OptionValueList m_geometry;
//        LVFS::Settings::OptionValue m_currentFile;
//    };
//
//
//    class Tabs : public LVFS::Settings::OptionList
//    {
//    public:
//        Tabs(Option *parent);
//
//        void add(const DirectoryView::Tab &tab);
//
//    protected:
//        virtual bool isSubOptionName(const QStringRef &name) const;
//        virtual Option *create();
//    };
//
//
//    class Settings : public LVFS::Settings::Scope
//    {
//    public:
//        typedef void (TabWidget::*Method)();
//
//    public:
//        Settings(const char *id, Scope *parent, TabWidget *object, Method save, Method load);
//
//        int activeTab() const { return m_activeTab.value(); }
//        void setActiveTab(int value) { m_activeTab.setValue(value); }
//
//        const Tabs &tabs() const { return m_tabs; }
//        Tabs &tabs() { return m_tabs; }
//
//    protected:
//        virtual void save(QXmlStreamWriter &stream) const;
//        virtual void load(QXmlStreamReader &stream);
//        virtual void loadDefault();
//
//    private:
//        LVFS::Settings::IntOption m_activeTab;
//        Tabs m_tabs;
//        TabWidget *m_object;
//        Method m_save;
//        Method m_load;
//    };

private:
    LVFS::Settings::IntOption m_tabIndex;
    LVFS::Settings::StringOption m_tabPath;
    LVFS::Settings::Scope m_tab;
    LVFS::Settings::List m_tabs;
    LVFS::Settings::IntOption m_currentTab;

private:
    typedef EFC::Map<QWidget *, LVFS::Interface::Holder> Container;

private:
//    Settings m_settings;
    Container m_views;
    LVFS::Interface::Holder &m_opposite;
    bool m_doNotRefreshTab;
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
};

#endif /* TABWIDGET_H_ */
