#include "idmqueryresultrootpathvalueitem.h"
#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"
#include "../../../../../filesystem/tasks/concrete/containers/filesysteminfolistitem.h"
#include "../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

QueryResultRootPathValueItem::QueryResultRootPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultPathItem(container, value->value().toString(), parent),
	m_value(value),
	m_container(container)
{
	lock(tr("Updating..."));
}

QString QueryResultRootPathValueItem::fileName() const
{
	return m_value->value().toString();
}

void QueryResultRootPathValueItem::open() const
{
	Application::desktopService()->open(m_container, this);
}

void QueryResultRootPathValueItem::update(const InfoItem *item)
{
	m_info = *item;

	if (item->isDir())
	{
		m_thisContainer = static_cast<const InfoListItem *>(item)->container()->open();

		for (InfoListItem::size_type i = 0, size = static_cast<const InfoListItem *>(item)->size(); i < size; ++i)
			m_items.push_back(new QueryResultPathValueItem(m_thisContainer.data(), static_cast<const InfoListItem *>(item)->at(i), this));
	}

	qSort(m_items.begin(), m_items.end(), ProxyModel::compareByFileNames);
}

IDM_PLUGIN_NS_END
