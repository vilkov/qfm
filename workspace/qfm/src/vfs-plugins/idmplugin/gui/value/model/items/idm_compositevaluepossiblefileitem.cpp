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
#include "idm_compositevaluepossiblefileitem.h"
#include "../../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePossibleFileItem::CompositeValuePossibleFileItem(const EntityValue::Holder &value, const SnapshotItem *source, IdmItem *parent) :
	CompositeValuePathItem(value, parent),
	m_source(source)
{}

QVariant CompositeValuePossibleFileItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_value->value();
		case Qt::DecorationRole:
			return m_source->info()->fileType()->icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		case Qt::ToolTipRole:
			return m_source->info()->fileType()->name();
	}

	return QVariant();
}

QString CompositeValuePossibleFileItem::fileName() const
{
	return m_source->info()->fileName();
}

bool CompositeValuePossibleFileItem::isFile() const
{
	return true;
}

void CompositeValuePossibleFileItem::open() const
{
	Application::open(m_source->container(), m_source->info());
}

IDM_PLUGIN_NS_END
