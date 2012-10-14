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
#include "colorconfigoption.h"


ColorConfigOption::ColorConfigOption(qint32 id, const QString &title, ConfigOption *parent) :
	ConfigOptionValue(id, title, QVariant(), parent),
	m_editor(0),
	m_listener(0)
{}

bool ColorConfigOption::isEnabled() const
{
	return m_label->isChecked();
}

void ColorConfigOption::setEnabled(bool value)
{
	m_label->setEnabled(value); setEditorValue(value);
}

QWidget *ColorConfigOption::createLabel(QWidget *parent, const QFont &font)
{
	m_label = new QCheckBox(title(), parent);
	m_label->setFont(font);

	return m_label;
}

QWidget *ColorConfigOption::createEditor(QWidget *parent)
{
	QScopedPointer<QWidget> widget(new QWidget(parent));
	QHBoxLayout *layout = new QHBoxLayout(widget.data());

	layout->setMargin(1);
	layout->setSpacing(1);
	layout->addWidget(m_editor = new QLabel(), 1);
	m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	m_editor->setAlignment(Qt::AlignCenter);
	layout->addWidget(m_button = new QPushButton(tr("...")));
	m_listener->connect(m_button, SIGNAL(clicked()), m_colorListenerSlot);
	m_listener->connect(m_label, SIGNAL(stateChanged(int)), m_checkListenerSlot);

	return widget.take();
}

QVariant ColorConfigOption::editorValue() const
{
	return QVariantList() << (m_label->checkState() == Qt::Checked) << m_editor->text();
}

void ColorConfigOption::setEditorValue(const QVariant &value)
{
	QVariantList list = value.toList();

	if (list.isEmpty())
		setEditorValue(false);
	else
	{
		if (list.at(0).isValid())
			setEditorValue(list.at(0).toBool());

		if (list.at(1).isValid())
			setEditorValue(qVariantValue<QColor>(list.at(1)));
	}
}

void ColorConfigOption::setLoadedEditorValue(const QVariant &value)
{
	QVariantList list = value.toList();

	if (list.isEmpty())
	{
		m_label->setChecked(false);
		setEditorValue(false);
		setEditorValue(QColor());
	}
	else
	{
		bool val = list.at(0).toBool();
		m_label->setChecked(val);
		setEditorValue(val);
		setEditorValue(qVariantValue<QColor>(list.at(1)));
	}
}

void ColorConfigOption::setListener(QObject *listener, const char *checkListenerSlot, const char *colorListenerSlot)
{
	m_listener = listener;
	m_checkListenerSlot = checkListenerSlot;
	m_colorListenerSlot = colorListenerSlot;
}

void ColorConfigOption::setEditorValue(bool value)
{
	m_editor->setEnabled(value);
	m_button->setEnabled(value);
}

void ColorConfigOption::setEditorValue(const QColor &value)
{
	m_editor->setText(value.name());
	m_editor->setPalette(QPalette(value));
	m_editor->setAutoFillBackground(true);
}
