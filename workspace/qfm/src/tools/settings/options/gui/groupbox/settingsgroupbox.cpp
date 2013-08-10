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
#include "settingsgroupbox.h"
#include "../../../../memory/memory_scopedpointer.h"

#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>


SETTINGS_NS_BEGIN

GroupBox::GroupBox(const QString &title, const QString &id, Option *parent) :
	Scope(id, parent),
	m_title(title)
{}

QWidget *GroupBox::createEditor()
{
	::Tools::Memory::ScopedPointer<QGroupBox> page(new QGroupBox(title()));
	QVBoxLayout *layout(new QVBoxLayout(page.data()));

	QGroupBox m_selectedPage;

	layout->setSpacing(5);
	layout->setMargin(5);

	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		layout->addWidget(m_guis.at(i)->createEditor());

	layout->addStretch(1);
	return page.take();
}

void GroupBox::restoreDefault()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->restoreDefault();
}

bool GroupBox::accept()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		if (!m_guis.at(i)->accept())
			return false;

	return true;
}

void GroupBox::reject()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->reject();
}

SETTINGS_NS_END
