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
#ifndef NESTEDPLAINDIALOG_H_
#define NESTEDPLAINDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "widgets/nesteddialog.h"


class QEventLoop;

class NestedPlainDialog : public QDialog, public NestedDialog
{
public:
	NestedPlainDialog(QWidget *parent = 0);

    virtual void accept();
	virtual void reject();

protected:
	/* NestedDialog */
	virtual void accepted();
	virtual void rejected();

	/* Message box */
	virtual void warning(const QString &text);
	virtual void warning(const QString &title, const QString &text);
	virtual void critical(const QString &text);
	virtual void critical(const QString &title, const QString &text);

	virtual int open(NestedWidget *widget);

	void setCentralWidget(BaseNestedWidget *widget);

private:
	QEventLoop *m_loop;
	BaseNestedWidget *m_mainWidget;
	NestedWidget *m_currentWidget;

private:
	QVBoxLayout m_vlayout;
	QSplitter m_splitter;
	QDialogButtonBox m_buttonBox;
};

#endif /* NESTEDPLAINDIALOG_H_ */
