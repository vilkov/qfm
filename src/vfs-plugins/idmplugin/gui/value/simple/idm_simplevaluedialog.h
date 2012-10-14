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
#ifndef SIMPLEVALUEDIALOG_H_
#define SIMPLEVALUEDIALOG_H_

#include "widgets/simplevaluewidget.h"
#include "../../../storage/structure/idm_databasetypes.h"
#include "../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace ::VFS::Plugins::Idm;

template <Database::EntityType EntityType>
class SimpleValueDialog : public NestedPlainDialog
{
public:
	typedef typename EntityValueType<EntityType>::type value_type;
	typedef ValueDialog<value_type>                    base_class;

public:
	SimpleValueDialog(const QString &title, QWidget *parent = 0) :
		NestedPlainDialog(parent)
	{
		setWindowTitle(title);
		setCentralWidget(&m_mainWidget);
	}

	value_type value() const { return m_mainWidget.value(); }

private:
	MainSimpleValueWidget<value_type> m_mainWidget;
};

#endif /* SIMPLEVALUEDIALOG_H_ */
