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
#include "emailconfigoption.h"


EmailConfigOption::EmailConfigOption(qint32 id, const QString &title, ConfigOption *parent) :
	ConfigOptionValueDefault(id, title, QString::fromLatin1("your.email@domain.com"), parent),
	m_editor(0),
	m_listener(0)
{}

bool EmailConfigOption::isEnabled() const
{
	return m_editor->isEnabled();
}

void EmailConfigOption::setEnabled(bool value)
{
	m_editor->setEnabled(value);
	m_button->setEnabled(value);
}

QWidget *EmailConfigOption::createEditor(QWidget *parent)
{
	QScopedPointer<QWidget> widget(new QWidget(parent));
	QHBoxLayout *layout = new QHBoxLayout(widget.data());

	layout->setMargin(1);
	layout->setSpacing(1);
	layout->addWidget(m_editor = new QLineEdit());
	layout->addWidget(m_button = new QPushButton(tr("Test")));
	m_listener->connect(m_button, SIGNAL(clicked()), m_listenerSlot);

	return widget.take();
}

QVariant EmailConfigOption::editorValue() const
{
	return m_editor->text();
}

void EmailConfigOption::setEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void EmailConfigOption::setLoadedEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void EmailConfigOption::setListener(QObject *listener, const char *listenerSlot)
{
	m_listener = listener; m_listenerSlot = listenerSlot;
}
