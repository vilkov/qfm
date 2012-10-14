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
#ifndef NESTEDWIDGET_H_
#define NESTEDWIDGET_H_

#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "basenestedwidget.h"


class NestedWidget : public BaseNestedWidget
{
public:
    enum DialogCode
    {
    	Rejected,
    	Accepted
    };

public:
	NestedWidget(NestedDialog *parent, const QString &title);
	virtual ~NestedWidget();

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);

	int exec();

	QString title() const { return m_groupbox.title(); }

protected:
	void setCentralWidget(QWidget *widget)
	{
		Q_ASSERT_X
		(
			m_hlayout.count() == 1,
			"NestedWidget::setCentralWidget",
			"Central widget already set!"
		);

		m_hlayout.addWidget(widget, 1);
	}

private:
    class Button : public QPushButton
    {
    public:
    	Button(const QString &text);

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;
    };

private:
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	Button m_ok;
	Button m_cancel;
};

#endif /* NESTEDWIDGET_H_ */
