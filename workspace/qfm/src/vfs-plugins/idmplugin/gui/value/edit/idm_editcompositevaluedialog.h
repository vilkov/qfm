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
#ifndef IDM_EDITCOMPOSITEVALUEDIALOG_H_
#define IDM_EDITCOMPOSITEVALUEDIALOG_H_

#include "../composite/idm_compositevaluedialog.h"


class EditCompositeValueDialog : public CompositeValueDialog
{
    Q_OBJECT

public:
    EditCompositeValueDialog(const IdmContainer &container, const EntityValue &value, QWidget *parent = 0);
};

#endif /* IDM_EDITCOMPOSITEVALUEDIALOG_H_ */
