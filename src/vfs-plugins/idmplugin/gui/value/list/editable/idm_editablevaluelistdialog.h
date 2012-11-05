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
#ifndef IDM_EDITABLEVALUELISTDIALOG_H_
#define IDM_EDITABLEVALUELISTDIALOG_H_

#include <tools/widgets/nestedplaindialog/nestedplaindialog.h>
#include "widgets/idm_editablevaluelistwidget.h"


using namespace ::VFS::Plugins::Idm;

class EditableValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

    virtual void accept();
	virtual void reject();

protected:
	void closeDbContext();
	EntityValue::Holder takeValue();
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventHandler<
				EventHandlerBase<
					EditableValueListDialog
				>
			> TreeViewHandler;

	void addValue();
	void removeValue();
	void setFocusToFilter();

private:
	TreeViewHandler m_handler;
	MainEditableValueListWidget m_widget;
};

#endif /* IDM_EDITABLEVALUELISTDIALOG_H_ */
