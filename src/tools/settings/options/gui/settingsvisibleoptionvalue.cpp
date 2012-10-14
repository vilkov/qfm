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
#include "settingsvisibleoptionvalue.h"
#include "../../../pointers/pscopedpointer.h"

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>


SETTINGS_NS_BEGIN

VisibleOptionValue::VisibleOptionValue(const QString &label, const QString &id, Option *parent, const QString &defaultValue) :
	OptionValue(id, parent, defaultValue),
	m_label(label)
{}

QWidget *VisibleOptionValue::createEditor()
{
	PScopedPointer<QWidget> page(new QWidget());
	QHBoxLayout *layout(new QHBoxLayout(page.data()));

	layout->setSpacing(1);
	layout->setMargin(1);

	layout->addWidget(new QLabel(m_label));
	layout->addWidget(m_editor = new QLineEdit(value()));

	return page.take();
}

void VisibleOptionValue::restoreDefault()
{
	m_editor->setText(defaultValue());
}

bool VisibleOptionValue::accept()
{
	if (!m_editor->text().isEmpty())
	{
		setValue(m_editor->text());
		return true;
	}

	return false;
}

void VisibleOptionValue::reject()
{}

SETTINGS_NS_END
