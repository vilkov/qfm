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
#include "stringdialog.h"
#include <QtGui/QMessageBox>


StringDialog::StringDialog(const QString &title, const QString &label, const QString &value, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_lineEdit(value, this),
	m_gridLayout(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    m_label.setFont(font);
    m_label.setText(label);

    m_gridLayout.setSpacing(6);
    m_gridLayout.setContentsMargins(6, 6, 6, 6);
    m_gridLayout.addWidget(&m_label, 0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit, 0, 1, 1, 1);
    m_gridLayout.addWidget(&m_buttonBox, 1, 0, 1, 2);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_lineEdit.selectAll();
}

void StringDialog::accept()
{
	if (check())
		QDialog::accept();
}

bool StringDialog::check()
{
	if (value().isEmpty())
	{
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
		return false;
	}
	else
		return true;
}
