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
#include "default_searchdialog.h"

#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
	m_layout(this),
	m_label(tr("File name pattern"), this),
	m_editor(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	m_hLayout.setSpacing(3);
	m_hLayout.setMargin(3);

	m_hLayout.addWidget(&m_label);
	m_hLayout.addWidget(&m_editor);

	m_layout.setSpacing(3);
	m_layout.setMargin(3);

	m_layout.addLayout(&m_hLayout);
	m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void SearchDialog::accept()
{
	if (m_editor.text().isEmpty())
		QMessageBox::warning(this, windowTitle(), tr("Enter file name pattern"));
	else
		QDialog::accept();
}

void SearchDialog::reject()
{
	QDialog::reject();
}

DEFAULT_PLUGIN_NS_END
