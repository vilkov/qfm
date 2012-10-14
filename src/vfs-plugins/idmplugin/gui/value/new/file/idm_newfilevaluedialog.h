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
#ifndef IDM_NEWFILEVALUEDIALOG_H_
#define IDM_NEWFILEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include "../composite/idm_newcompositevaluedialog.h"


class NewFileValueDialog : public NewCompositeValueDialog
{
	Q_OBJECT

public:
	NewFileValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();
};

#endif /* IDM_NEWFILEVALUEDIALOG_H_ */
