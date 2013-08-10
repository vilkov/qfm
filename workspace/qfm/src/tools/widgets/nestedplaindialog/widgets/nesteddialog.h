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
#ifndef NESTEDDIALOG_H_
#define NESTEDDIALOG_H_

#include "nestedwidget.h"


class BaseNestedDialog
{
public:
	virtual ~BaseNestedDialog();

protected:
	void accept(NestedWidget *widget) { widget->accept(); }
	void reject(NestedWidget *widget) { widget->reject(); }
};


class IntermediateNestedDialog : public BaseNestedDialog
{
protected:
	friend class BaseNestedWidget;
	virtual void accepted() = 0;
	virtual void rejected() = 0;

	/* Message box */
	virtual void warning(const QString &text) = 0;
	virtual void warning(const QString &title, const QString &text) = 0;
	virtual void critical(const QString &text) = 0;
	virtual void critical(const QString &title, const QString &text) = 0;
};


class NestedDialog : public IntermediateNestedDialog
{
protected:
	friend class NestedWidget;
	virtual int open(NestedWidget *widget) = 0;
};

#endif /* NESTEDDIALOG_H_ */
