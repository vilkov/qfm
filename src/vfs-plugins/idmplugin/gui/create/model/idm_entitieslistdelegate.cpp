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
#include "idm_entitieslistdelegate.h"
#include "items/idm_entitieslistmodelitem.h"

#include <tools/memory/memory_scopedpointer.h>

#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>


IDM_PLUGIN_NS_BEGIN

EntitiesListDelegate::EntitiesListDelegate(const IdmContainer &container, QObject *parent) :
	Delegate(parent),
    m_container(container)
{}

QWidget *EntitiesListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
		{
			return new QLineEdit(parent);
		}
		case 1:
		{
			::Tools::Memory::ScopedPointer<QComboBox> editor(new QComboBox(parent));

			for (IdmContainer::size_type i = 0, size = m_container.size(); i < size; ++i)
		    	editor->addItem(m_container.at(i)->name(), m_container.at(i)->id());

			editor->setCurrentIndex(0);
			return editor.take();
		}
	}

	return 0;
}

void EntitiesListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
		{
			static_cast<QLineEdit*>(editor)->setText(static_cast<IdmEntitiesListItem*>(index.internalPointer())->property().name);
			break;
		}
		case 1:
		{
			IdmContainer::size_type idx = m_container.indexOf(static_cast<IdmEntitiesListItem*>(index.internalPointer())->entity()->id());

			if (idx != IdmContainer::InvalidIndex)
				static_cast<QComboBox*>(editor)->setCurrentIndex(idx);

			break;
		}
	}
}

void EntitiesListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
		{
			QString text = static_cast<QLineEdit*>(editor)->text();

			if (!text.isEmpty())
				static_cast<IdmEntitiesListItem*>(index.internalPointer())->setName(text);

			break;
		}
		case 1:
		{
			IdmContainer::size_type idx = m_container.indexOf(static_cast<QComboBox*>(editor)->itemData(static_cast<QComboBox*>(editor)->currentIndex(), Qt::UserRole).toInt());

			if (idx != IdmContainer::InvalidIndex)
				static_cast<IdmEntitiesListItem*>(index.internalPointer())->setEntity(m_container.at(idx));

			break;
		}
	}
}

IDM_PLUGIN_NS_END
