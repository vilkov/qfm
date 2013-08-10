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
#include "settingsradiobutton.h"

#include <QtGui/QRadioButton>


SETTINGS_NS_BEGIN

RadioButton::RadioButton(const QString &title, const QString &id, Option *parent, bool defaultValue) :
	OptionValue(id, parent, defaultValue ? QChar(L'1') : QChar(L'0')),
	m_title(title),
	m_editor(NULL)
{}

QWidget *RadioButton::createEditor()
{
	m_editor = new QRadioButton(m_title);
	static_cast<QRadioButton *>(m_editor)->setChecked(value());
	return m_editor;
}

void RadioButton::restoreDefault()
{
	static_cast<QRadioButton *>(m_editor)->setChecked(defaultValue().at(0) == QChar(L'1'));
}

bool RadioButton::accept()
{
	setValue(static_cast<QRadioButton *>(m_editor)->isChecked());
	return true;
}

void RadioButton::reject()
{}

SETTINGS_NS_END
