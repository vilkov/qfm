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
#include "idm_ratingvaluewidget.h"


RatingValueWidgetPrivate::RatingValueWidgetPrivate() :
	m_result(1),
	m_button1(tr("1 - Bad"), this),
	m_button2(tr("2 - So-so"), this),
	m_button3(tr("3 - Good"), this),
	m_button4(tr("4 - Very good"), this),
	m_button5(tr("5 - Excellent"), this),
	m_verticatLayout(this)
{
	m_button1.setChecked(true);

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_button1);
	m_verticatLayout.addWidget(&m_button2);
	m_verticatLayout.addWidget(&m_button3);
	m_verticatLayout.addWidget(&m_button4);
	m_verticatLayout.addWidget(&m_button5);

    connect(&m_button1, SIGNAL(toggled(bool)), this, SLOT(button1Click(bool)));
    connect(&m_button2, SIGNAL(toggled(bool)), this, SLOT(button2Click(bool)));
    connect(&m_button3, SIGNAL(toggled(bool)), this, SLOT(button3Click(bool)));
    connect(&m_button4, SIGNAL(toggled(bool)), this, SLOT(button4Click(bool)));
    connect(&m_button5, SIGNAL(toggled(bool)), this, SLOT(button5Click(bool)));
}

void RatingValueWidgetPrivate::setFocus()
{
	switch (m_result)
	{
		case 1:
			m_button1.setFocus();
			break;

		case 2:
			m_button2.setFocus();
			break;

		case 3:
			m_button3.setFocus();
			break;

		case 4:
			m_button4.setFocus();
			break;

		case 5:
			m_button5.setFocus();
			break;

		default:
			QWidget::setFocus();
			break;
	}
}

void RatingValueWidgetPrivate::button1Click(bool checked)
{
	if (checked)
		m_result = 1;
}

void RatingValueWidgetPrivate::button2Click(bool checked)
{
	if (checked)
		m_result = 2;
}

void RatingValueWidgetPrivate::button3Click(bool checked)
{
	if (checked)
		m_result = 3;
}

void RatingValueWidgetPrivate::button4Click(bool checked)
{
	if (checked)
		m_result = 4;
}

void RatingValueWidgetPrivate::button5Click(bool checked)
{
	if (checked)
		m_result = 5;
}

MainRatingValueWidget::MainRatingValueWidget(NestedDialog *parent) :
	BaseNestedWidget(parent)
{}

QWidget *MainRatingValueWidget::centralWidget()
{
	return &m_private;
}

void MainRatingValueWidget::setReadOnly(bool value)
{
	m_private.setEnabled(!value);
}

void MainRatingValueWidget::setFocus()
{
	m_private.setFocus();
}

RatingValueWidget::RatingValueWidget(NestedDialog *parent, const QString &title) :
	NestedWidget(parent, title)
{
	setCentralWidget(&m_private);
}

void RatingValueWidget::setFocus()
{
	m_private.setFocus();
}
