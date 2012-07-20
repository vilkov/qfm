#include "idmqueryresultpathvalueitem.h"
#include "../../../../../application.h"

#include <vfs/model/vfs_proxymodel.h>


IDM_PLUGIN_NS_BEGIN

inline static bool compareByFileNames(const QueryResultItem::Holder &v1, const QueryResultItem::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultPathItem>()->info(), v2.as<QueryResultPathItem>()->info());
}


QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, SnapshotItem *item, Base *parent) :
	QueryResultPathItem(item->info(), parent),
	m_container(container)
{
	if (m_info->isDir())
	{
		m_thisContainer = item->thisContainer().take();

		for (SnapshotItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
			m_items.push_back(QueryResultItem::Holder(new QueryResultPathValueItem(m_thisContainer.data(), (*i), this)));
	}

	qSort(m_items.begin(), m_items.end(), compareByFileNames);
}

bool QueryResultPathValueItem::isRootPathValue()
{
	return false;
}

void QueryResultPathValueItem::open() const
{
	Application::desktopService()->open(m_container, info());
}

IDM_PLUGIN_NS_END
