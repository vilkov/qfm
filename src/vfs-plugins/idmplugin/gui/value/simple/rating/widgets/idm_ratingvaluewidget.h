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
#ifndef IDM_RATINGVALUEWIDGET_H_
#define IDM_RATINGVALUEWIDGET_H_

#include <QtGui/QRadioButton>
#include "../../../../../storage/values/idm_entityvalue.h"
#include "../../../../../../../tools/widgets/nestedplaindialog/widgets/nestedwidget.h"


using namespace ::VFS::Plugins::Idm;

class RatingValueWidgetPrivate : public QWidget
{
	Q_OBJECT

public:
	RatingValueWidgetPrivate();

    int value() const { return m_result; }
    void setFocus();

private Q_SLOTS:
	void button1Click(bool checked);
	void button2Click(bool checked);
	void button3Click(bool checked);
	void button4Click(bool checked);
	void button5Click(bool checked);

private:
	int m_result;
    QRadioButton m_button1;
    QRadioButton m_button2;
    QRadioButton m_button3;
    QRadioButton m_button4;
    QRadioButton m_button5;
	QVBoxLayout m_verticatLayout;
};


class MainRatingValueWidget : public BaseNestedWidget
{
public:
	MainRatingValueWidget(NestedDialog *parent);

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);
	virtual void setFocus();

    int value() const { return m_private.value(); }

private:
	RatingValueWidgetPrivate m_private;
};


class RatingValueWidget : public NestedWidget
{
public:
	RatingValueWidget(NestedDialog *parent, const QString &title);

	/* BaseNestedWidget */
	virtual void setFocus();

    int value() const { return m_private.value(); }

private:
	RatingValueWidgetPrivate m_private;
};

#endif /* IDM_RATINGVALUEWIDGET_H_ */
