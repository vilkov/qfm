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
#include "passwordconfigoption.h"


PasswordConfigOption::PasswordConfigOption(qint32 id, const QString &title, ConfigOption *parent) :
	ConfigOptionValueDefault(id, title, QVariant(), parent),
	m_editor(0)
{}

bool PasswordConfigOption::isEnabled() const
{
	return m_editor->isEnabled();
}

void PasswordConfigOption::setEnabled(bool value)
{
	m_editor->setEnabled(value);
}

QWidget *PasswordConfigOption::createEditor(QWidget *parent)
{
	m_editor = new QLineEdit(parent);
	static_cast<QLineEdit*>(m_editor)->setEchoMode(QLineEdit::Password);

	return m_editor;
}

QVariant PasswordConfigOption::editorValue() const
{
	return m_editor->text();
}

void PasswordConfigOption::setEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void PasswordConfigOption::setLoadedEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}
