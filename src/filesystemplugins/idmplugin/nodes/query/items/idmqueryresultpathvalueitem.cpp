#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"
#include "../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, WrappedNodeItem *item, Base *parent) :
	QueryResultPathItem(item->info(), parent),
	m_container(container)
{
	if (m_info->isDir())
	{
		m_thisContainer = item->container()->open();

		for (WrappedNodeItem::size_type i = 0, size = item->size(); i < size; ++i)
			m_items.push_back(new QueryResultPathValueItem(m_thisContainer.data(), item->at(i), this));
	}

	qSort(m_items.begin(), m_items.end(), ProxyModel::compareByFileNames);
}

void QueryResultPathValueItem::open() const
{
	Application::desktopService()->open(m_container, this);
}

IDM_PLUGIN_NS_END
