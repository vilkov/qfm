#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"
#include "../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

inline static bool compareByFileNames(const QueryResultItem::Holder &v1, const QueryResultItem::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultPathItem>(), v2.as<QueryResultPathItem>());
}


QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, WrappedNodeItem *item, Base *parent) :
	QueryResultPathItem(item->info(), parent),
	m_container(container)
{
	if (m_info->isDir())
	{
		m_thisContainer = item->thisContainer().take();

		for (WrappedNodeItem::size_type i = 0, size = item->size(); i < size; ++i)
			m_items.push_back(QueryResultItem::Holder(new QueryResultPathValueItem(m_thisContainer.data(), item->at(i), this)));
	}

	qSort(m_items.begin(), m_items.end(), compareByFileNames);
}

bool QueryResultPathValueItem::isRootPathValue()
{
	return false;
}

void QueryResultPathValueItem::open() const
{
	Application::desktopService()->open(m_container, this);
}

IDM_PLUGIN_NS_END
