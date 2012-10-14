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
#include "idm_queryresultrootpathvalueitem.h"
#include "idm_queryresultpathvalueitem.h"
#include "../../../interfaces/idm_invalidfileinfo.h"
#include "../../../../../application.h"

#include <vfs/model/vfs_proxymodel.h>


IDM_PLUGIN_NS_BEGIN

inline static bool compareByFileNames(const Item::Holder &v1, const Item::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultPathItem>()->info(), v2.as<QueryResultPathItem>()->info());
}


QueryResultRootPathValueItem::QueryResultRootPathValueItem(const IFileContainer *container, const EntityValue::Holder &value, Base *parent) :
	QueryResultPathItem(container, value->value().toString(), parent),
	m_value(value),
	m_container(container)
{
	lock(tr("Updating..."));
}

bool QueryResultRootPathValueItem::isRootPathValue()
{
	return true;
}

void QueryResultRootPathValueItem::open() const
{
	QString error = m_value->value().toString();
	IFileContainer::Holder folder(m_container->create(error.mid(0, error.lastIndexOf(QChar(L'/'))), error));

	if (folder)
		Application::open(folder, info());
}

void QueryResultRootPathValueItem::update(SnapshotItem *item)
{
	m_items.clear();

	if (item)
	{
		m_info = item->info().take();

		if (m_info->isDir())
		{
			m_thisContainer = item->thisContainer().take();

			for (SnapshotItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
				m_items.push_back(QueryResultItem::Holder(new QueryResultPathValueItem(m_thisContainer.data(), (*i), this)));
		}

		qSort(m_items.begin(), m_items.end(), compareByFileNames);
	}
	else
		m_info = new InvalidInfo(m_value->value().toString());
}

IDM_PLUGIN_NS_END
