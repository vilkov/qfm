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
#include "basenestedwidget.h"
#include "nesteddialog.h"


BaseNestedWidget::BaseNestedWidget(NestedDialog *parent) :
	m_parent(parent)
{}

BaseNestedWidget::~BaseNestedWidget()
{}

void BaseNestedWidget::warning(const QString &text)
{
	m_parent->warning(text);
}

void BaseNestedWidget::warning(const QString &title, const QString &text)
{
	m_parent->warning(title, text);
}

void BaseNestedWidget::critical(const QString &text)
{
	m_parent->critical(text);
}

void BaseNestedWidget::critical(const QString &title, const QString &text)
{
	m_parent->critical(title, text);
}

void BaseNestedWidget::accept()
{
	m_parent->accepted();
}

void BaseNestedWidget::reject()
{
	m_parent->rejected();
}
