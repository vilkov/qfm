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
#include "idm_queryresultpathitem.h"
#include "../../../interfaces/idm_invalidfileinfo.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathItem::QueryResultPathItem(IFileInfo::Holder &info, Base *parent) :
	QueryResultItem(parent),
	m_info(info.take()),
    m_location(m_info->fileName()),
	m_node(NULL)
{}

QueryResultPathItem::QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent) :
	QueryResultItem(parent),
	m_info(new InvalidInfo(fileName)),
    m_location(m_info->fileName()),
	m_node(NULL)
{}

QueryResultPathItem::Base *QueryResultPathItem::at(Base::size_type index) const
{
	return m_items.at(index).data();
}

QueryResultPathItem::Base::size_type QueryResultPathItem::size() const
{
	return m_items.size();
}

QueryResultPathItem::Base::size_type QueryResultPathItem::indexOf(Base *item) const
{
	QueryResultItem::Holder holder(static_cast<QueryResultPathItem *>(item));
	return m_items.indexOf(holder);
}

QVariant QueryResultPathItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_info->fileName().as<QString>();
		case Qt::DecorationRole:
			if (isLocked())
				return lockIcon();
			else
				return m_info->fileType()->icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		case Qt::ToolTipRole:
			if (isLocked())
				return lockReason();
			else
				break;
	}

	return QVariant();
}

bool QueryResultPathItem::isRoot()
{
	return false;
}

bool QueryResultPathItem::isProperty()
{
	return false;
}

bool QueryResultPathItem::isValue()
{
	return false;
}

bool QueryResultPathItem::isPath()
{
	return true;
}

IDM_PLUGIN_NS_END
