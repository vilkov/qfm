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
#include "settingsvisibleoptionfont.h"
#include "../../../memory/memory_scopedpointer.h"
#include "../../../events/qt/imp/mouseeventsource.h"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>


SETTINGS_NS_BEGIN

VisibleOptioinFont::VisibleOptioinFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue) :
	OptionFont(id, parent, defaultValue),
	m_eventHandler(this),
	m_label(label)
{
	m_eventHandler.registerMouseReleaseEventHandler(&VisibleOptioinFont::chooseFontEvent);
}

QWidget *VisibleOptioinFont::createEditor()
{
	typedef Events::MouseReleaseEventSource<Events::EventSourceBase<QPushButton> > Button;
	::Tools::Memory::ScopedPointer<QWidget> page(new QWidget());
	QHBoxLayout *layout(new QHBoxLayout(page.data()));

	layout->setSpacing(3);
	layout->setMargin(1);

	layout->addWidget(new QLabel(m_label));

	layout->addWidget(m_editor = new QLabel(toString(m_currentFont = OptionFont::font())), 1);
	m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	m_editor->setAlignment(Qt::AlignCenter);
	m_editor->setFont(m_currentFont);

	layout->addWidget(new Button(&m_eventHandler, QString::fromLatin1("...")));

	return page.take();
}

void VisibleOptioinFont::restoreDefault()
{
	m_editor->setText(toString(m_currentFont = OptionFont::defaultValue()));
	m_editor->setFont(m_currentFont);
}

bool VisibleOptioinFont::accept()
{
	if (m_currentFont != font())
	{
		setFont(m_currentFont);
		applyFont(m_currentFont);
	}

	return true;
}

void VisibleOptioinFont::reject()
{}

void VisibleOptioinFont::chooseFontEvent()
{
	if (chooseFont(m_currentFont))
	{
		m_editor->setText(toString(m_currentFont));
		m_editor->setFont(m_currentFont);
	}
}

SETTINGS_NS_END
