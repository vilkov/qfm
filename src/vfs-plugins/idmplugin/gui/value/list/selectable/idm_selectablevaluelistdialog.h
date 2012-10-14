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
#ifndef IDM_SELECTABLEVALUELISTDIALOG_H_
#define IDM_SELECTABLEVALUELISTDIALOG_H_

#include "../editable/idm_editablevaluelistdialog.h"


using namespace ::VFS::Plugins::Idm;

class SelectableValueListDialog : public EditableValueListDialog
{
	Q_OBJECT

public:
	SelectableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

	EntityValue::Holder takeValue() { return EditableValueListDialog::takeValue(); }
    virtual void accept();
};

#endif /* IDM_SELECTABLEVALUELISTDIALOG_H_ */
