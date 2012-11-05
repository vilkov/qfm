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
#include "foldersview.h"
#include <tools/pointers/pscopedpointer.h>


FoldersView::FoldersView(const QString &id, ::Tools::Settings::Scope *settings, FoldersViewRef other, QWidget *parent) :
	QWidget(parent),
    m_settings(id, settings, this, &FoldersView::save, &FoldersView::load),
	m_doNotRefreshTab(true),
    m_layout(this),
    m_tabWidget(this),
	m_other(other)
{
	m_layout.setMargin(3);
	m_layout.setSpacing(1);
	m_layout.addWidget(&m_tabWidget);
	m_layout.setStretchFactor(&m_tabWidget, 1);

	connect(&m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshTab(int)));
}

void FoldersView::refresh()
{
	doRefresh(m_tabWidget.currentWidget());
}

void FoldersView::updateTitle(QWidget *widget, const QString &fileName)
{
	m_tabWidget.setTabText(m_tabWidget.indexOf(widget), fileName);
}

void FoldersView::openInNewTab(::VFS::INode *node, const QModelIndex &index, const QList<qint32> &geometry)
{
	PScopedPointer<DirectoryView> widget(new DirectoryView(node, index, geometry, this));
	m_doNotRefreshTab = true;
	m_tabWidget.setCurrentIndex(m_tabWidget.addTab(widget.data(), widget->title()));
	widget.take()->setFocus();
}

void FoldersView::closeCurrentTab()
{
	if (m_tabWidget.count() > 1)
	{
		PScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
		m_tabWidget.removeTab(m_tabWidget.currentIndex());
		static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setFocus();
	}
}

void FoldersView::setFocus()
{
	static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setFocus();
}

void FoldersView::setCurrentDirectory(const QString &filePath)
{
	static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setCurrentDirectory(filePath);
}

void FoldersView::skipOneRefreshTab()
{
	m_doNotRefreshTab = true;
}

void FoldersView::refreshTab(int index)
{
	doRefresh(m_tabWidget.widget(index));
}

void FoldersView::doRefresh(QWidget *tab)
{
	if (m_doNotRefreshTab)
		m_doNotRefreshTab = false;
	else
		static_cast<DirectoryView*>(tab)->refresh();
}

void FoldersView::save()
{
	m_settings.setActiveTab(m_tabWidget.currentIndex());

	for (qint32 i = 0, size = m_tabWidget.count(); i < size; ++i)
		m_settings.tabs().add(static_cast<DirectoryView *>(m_tabWidget.widget(i))->tab());
}

void FoldersView::load()
{
	if (m_settings.tabs().isEmpty())
	{
		PScopedPointer<DirectoryView> widget(new DirectoryView(DirectoryView::defaultPath(), this));
		m_tabWidget.addTab(widget.data(), widget->title());
		widget.release();
	}
	else
	{
		Tab *tab;
		PScopedPointer<DirectoryView> widget;

		for (Tabs::const_iterator i = m_settings.tabs().begin(), end = m_settings.tabs().end(); i != end; ++i)
		{
			tab = (*i)->as<Tab>();
			widget.reset(new DirectoryView(tab->path(), tab->sort().column(), tab->sort().order(), tab->geometry(), tab->currentFile(), this));
			m_tabWidget.addTab(widget.data(), widget->title());
			widget.release();
		}

		m_tabWidget.setCurrentIndex(m_settings.activeTab());
		m_settings.tabs().clear();
	}
}

FoldersView::Sort::Sort(Option *parent) :
	Scope(QString::fromLatin1("Sort"), parent),
	m_column(QString::fromLatin1("Column"), this),
	m_order(QString::fromLatin1("Order"), this)
{
	manage(&m_column);
	manage(&m_order);
}

FoldersView::Sort::Sort(const DirectoryView::Tab::Sort &sort, Option *parent) :
	Scope(QString::fromLatin1("Sort"), parent),
	m_column(QString::fromLatin1("Column"), this),
	m_order(QString::fromLatin1("Order"), this)
{
	manage(&m_column);
	manage(&m_order);

	m_column.setValue(QString::number(sort.column));
	m_order.setValue(QString::number(sort.order));
}

FoldersView::Tab::Tab(Option *parent) :
	Scope(QString::fromLatin1("Tab"), parent),
	m_path(QString::fromLatin1("Path"), this),
	m_sort(this),
	m_geometry(QString::fromLatin1("Geometry"), this),
	m_currentFile(QString::fromLatin1("CurrentFile"), this)
{
	manage(&m_path);
	manage(&m_sort);
	manage(&m_geometry);
	manage(&m_currentFile);
}

FoldersView::Tab::Tab(const DirectoryView::Tab &tab, Option *parent) :
	Scope(QString::fromLatin1("Tab"), parent),
	m_path(QString::fromLatin1("Path"), this),
	m_sort(tab.sort, this),
	m_geometry(QString::fromLatin1("Geometry"), this),
	m_currentFile(QString::fromLatin1("CurrentFile"), this)
{
	manage(&m_path);
	manage(&m_sort);
	manage(&m_geometry);
	manage(&m_currentFile);

	m_path.setValue(tab.path);
	m_currentFile.setValue(tab.currentFile);

	for (DirectoryView::Geometry::size_type i = 0, size = tab.geometry.size(); i < size; ++i)
		m_geometry.add(QString::number(tab.geometry.at(i)));
}

DirectoryView::Geometry FoldersView::Tab::geometry() const
{
	DirectoryView::Geometry res;
	res.reserve(m_geometry.size());

	for (::Tools::Settings::OptionValueList::size_type i = 0, size = m_geometry.size(); i < size; ++i)
		res.push_back(m_geometry.at(i).toInt());

	return res;
}

FoldersView::Tabs::Tabs(Option *parent) :
	OptionList(QString::fromLatin1("Tabs"), parent)
{}

void FoldersView::Tabs::add(const DirectoryView::Tab &tab)
{
	OptionList::add(new Tab(tab, this));
}

bool FoldersView::Tabs::isSubOptionName(const QStringRef &name) const
{
	static const QString optionName = QString::fromLatin1("Tab");
	return name == optionName;
}

::Tools::Settings::Option *FoldersView::Tabs::create()
{
	return new Tab(this);
}

FoldersView::Settings::Settings(const QString &id, Scope *parent, FoldersView *object, Method save, Method load) :
	Scope(id, parent),
	m_activeTab(QString::fromLatin1("ActiveTab"), this),
	m_tabs(this),
	m_object(object),
	m_save(save),
	m_load(load)
{
	parent->manage(this);
	manage(&m_activeTab);
	manage(&m_tabs);
}

void FoldersView::Settings::save(QXmlStreamWriter &stream) const
{
	(m_object->*m_save)();
	Scope::save(stream);
}

void FoldersView::Settings::load(QXmlStreamReader &stream)
{
	Scope::load(stream);
	(m_object->*m_load)();
}

void FoldersView::Settings::loadDefault()
{
	Scope::loadDefault();
	(m_object->*m_load)();
}
