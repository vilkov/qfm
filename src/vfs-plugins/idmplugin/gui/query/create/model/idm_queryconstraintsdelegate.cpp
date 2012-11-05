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
#include "idm_queryconstraintsdelegate.h"
#include "../../../../storage/constraints/group/idm_groupconstraint.h"

#include <tools/pointers/pscopedpointer.h>
#include <QtGui/QComboBox>


IDM_PLUGIN_NS_BEGIN

QueryConstraintsDelegate::QueryConstraintsDelegate(QObject *parent) :
	Delegate(parent)
{}

QWidget *QueryConstraintsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			PScopedPointer<QComboBox> editor(new QComboBox(parent));

			editor->addItem(GroupConstraint::typeToString(GroupConstraint::And), GroupConstraint::And);
	    	editor->addItem(GroupConstraint::typeToString(GroupConstraint::Or), GroupConstraint::Or);
			editor->setCurrentIndex(0);

			return editor.take();
		}
	}

	return 0;
}

void QueryConstraintsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			static_cast<QComboBox*>(editor)->setCurrentIndex(static_cast<GroupConstraint*>(index.internalPointer())->type());
			break;
		}
	}
}

void QueryConstraintsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			static_cast<GroupConstraint*>(index.internalPointer())->setType(static_cast<GroupConstraint::Type>(static_cast<QComboBox*>(editor)->itemData(static_cast<QComboBox*>(editor)->currentIndex(), Qt::UserRole).toInt()));
			break;
		}
	}
}

IDM_PLUGIN_NS_END
