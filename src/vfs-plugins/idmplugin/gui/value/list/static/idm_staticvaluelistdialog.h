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
#ifndef IDM_STATICVALUELISTDIALOG_H_
#define IDM_STATICVALUELISTDIALOG_H_

#include <tools/widgets/nestedplaindialog/nestedplaindialog.h>
#include "../editable/widgets/idm_editablevaluelistwidget.h"


using namespace ::VFS::Plugins::Idm;

class StaticValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);
	virtual ~StaticValueListDialog();

	EntityValue::Holder takeValue();
    virtual void accept();

protected:
    QModelIndex currentIndex() const;

private:
	typedef MouseDoubleClickEventHandler<
                KeyboardEventHandler<
                    EventHandlerBase<
                        StaticValueListDialog
                    >
                >
	        > TreeViewHandler;

	void setFocusToFilter();

private:
	TreeViewHandler m_handler;
	MainEditableValueListWidget m_widget;
};

#endif /* IDM_STATICVALUELISTDIALOG_H_ */
