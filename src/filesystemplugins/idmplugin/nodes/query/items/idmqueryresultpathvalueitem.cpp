#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"
#include "../../../../../filesystem/tasks/concrete/containers/filesysteminfolistitem.h"
#include "../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, const InfoItem *item, Base *parent) :
	QueryResultPathItem(item, parent),
	m_container(container)
{
	if (item->isDir())
	{
		m_thisContainer = static_cast<const InfoListItem *>(item)->container()->open();

		for (InfoListItem::size_type i = 0, size = static_cast<const InfoListItem *>(item)->size(); i < size; ++i)
			m_items.push_back(new QueryResultPathValueItem(m_thisContainer.data(), static_cast<const InfoListItem *>(item)->at(i), this));
	}

	qSort(m_items.begin(), m_items.end(), ProxyModel::compareByFileNames);
}

void QueryResultPathValueItem::open() const
{
	Application::desktopService()->open(m_container, this);
}

IDM_PLUGIN_NS_END
