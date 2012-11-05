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
#ifndef IDM_COMPOSITEVALUEDIALOG_H_
#define IDM_COMPOSITEVALUEDIALOG_H_

#include <tools/widgets/nestedplaindialog/nestedplaindialog.h>
#include "widgets/idm_compositevaluewidget.h"


class CompositeValueDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	CompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, QWidget *parent = 0);
	CompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();

protected:
    QModelIndex currentIndex() const { return m_mainWidget.currentIndex(); }

    const IdmContainer &container() const { return m_mainWidget.container(); }
	IdmContainer &container() { return m_mainWidget.container(); }

	const CompositeValueModel &model() const { return m_mainWidget.model(); }
	CompositeValueModel &model() { return m_mainWidget.model(); }

	void doOpenFile(const QModelIndex &index) { m_mainWidget.open(index); }
	void doAddValue(const QModelIndex &index) { m_mainWidget.addValue(index); }
	void doRemoveValue(const QModelIndex &index) { m_mainWidget.removeValue(index); }

private:
	void openFile1();
	void openFile2();

private:
	typedef MouseDoubleClickEventHandler<
				KeyboardEventHandler<
					EventHandlerBase<
						CompositeValueDialog
					>
				>
			> TreeViewHandler;

private:
	TreeViewHandler m_handler;
	MainCompositeValueWidget m_mainWidget;
};

#endif /* IDM_COMPOSITEVALUEDIALOG_H_ */
