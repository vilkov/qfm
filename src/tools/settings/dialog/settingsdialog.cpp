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
#include "settingsdialog.h"
#include "../configoptions/configoptionvalue.h"


Dialog::Dialog(const QString &title, DialogSettings &settings, const Pages &pages, QWidget *parent) :
	QDialog(parent),
	m_settings(settings),
	m_model(pages, this),
	m_currentPage(pages.at(0)),
	m_layout(this),
	m_splitter(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);
	restoreGeometry(m_settings.geometry());
	setFont(m_settings.font());

	m_pagesView.setModel(&m_model);
	m_pagesView.setHeaderHidden(true);
	m_pagesView.selectionModel()->select(m_model.index(0, 0), QItemSelectionModel::ClearAndSelect);

	m_currentPageWidget = m_currentPage->createEditor();
	m_editedPages.insert(m_currentPage);

	m_splitter.setChildrenCollapsible(false);
	m_splitter.addWidget(&m_pagesView);
	m_splitter.addWidget(m_currentPageWidget);
	restoreSplitterState();

	m_layout.setSpacing(3);
	m_layout.setMargin(1);

	m_layout.addWidget(&m_splitter);
	m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(&m_buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(clicked(QAbstractButton *)));
    connect(&m_pagesView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked(const QModelIndex &)));
}

Dialog::~Dialog()
{
	m_settings.setSplitterState(m_splitter.saveState());
	m_settings.setGeometry(saveGeometry());
}

void Dialog::accept()
{
	for (PagesSet::const_iterator i = m_editedPages.constBegin(), end = m_editedPages.constEnd(); i != end; ++i)
		if (!(*i)->accept())
			return;

	QDialog::accept();
}

void Dialog::reject()
{
	for (PagesSet::const_iterator i = m_editedPages.constBegin(), end = m_editedPages.constEnd(); i != end; ++i)
		(*i)->reject();

	QDialog::reject();
}

void Dialog::restoreSplitterState()
{
	QByteArray splitterState(m_settings.splitterState());

	if (splitterState.isEmpty())
		m_splitter.setSizes(QList<int>() << width() * 1/4 << width() * 3/4);
	else
		m_splitter.restoreState(splitterState);
}

void Dialog::clicked(const QModelIndex &index)
{
	Page *page = m_model.pageAt(index);

	if (page != m_currentPage)
	{
		delete m_currentPageWidget;

		m_currentPage = page;
		m_editedPages.insert(m_currentPage);

		m_splitter.addWidget(m_currentPageWidget = m_currentPage->createEditor());
		restoreSplitterState();
	}
}

void Dialog::clicked(QAbstractButton *button)
{
	if (m_buttonBox.standardButton(button) == QDialogButtonBox::RestoreDefaults)
		m_currentPage->restoreDefault();
}


SettingsDialog::SettingsDialog(const QString &title, const SettingsList &settings, const ConstraintsList &constraints, QWidget *parent) :
	QDialog(parent),
    m_tabs(this),
	m_layout(this),
	m_buttonBox(this),
	m_constraints(constraints)
{
    setWindowTitle(title);

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_layout.setSpacing(6);
    m_layout.setContentsMargins(6, 6, 6, 6);
    m_layout.addWidget(&m_tabs);
    m_layout.addWidget(&m_buttonBox);

    QFont font = createFont();
	for (SettingsList::size_type i = 0, size = settings.size(); i < size; ++i)
		m_tabs.addTab(settings.at(i)->createGroup(font), settings.at(i)->title());
}

void SettingsDialog::accept()
{
	for (ConstraintsList::size_type i = 0, size = m_constraints.size(); i < size; ++i)
		if (!m_constraints.at(i)->check(this))
			return;

	QDialog::accept();
}

QFont SettingsDialog::createFont() const
{
    QFont font;
    font.setBold(true);
    font.setWeight(75);

    return font;
}
