#include "idmqueryresultrootpathvalueitem.h"
#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"
#include "../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

inline static bool compareByFileNames(const NodeItem::Holder &v1, const NodeItem::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultPathItem>(), v2.as<QueryResultPathItem>());
}


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

bool QueryResultRootPathValueItem::isRootPathValue()
{
	return true;
}

void QueryResultRootPathValueItem::open() const
{
	Application::desktopService()->open(m_container, this);
}

void QueryResultRootPathValueItem::update(WrappedNodeItem *item)
{
	m_items.clear();

	m_info = item->info().take();

	if (m_info->isDir())
	{
		m_thisContainer = item->thisContainer().take();

		for (WrappedNodeItem::size_type i = 0, size = item->size(); i < size; ++i)
			m_items.push_back(QueryResultItem::Holder(new QueryResultPathValueItem(m_thisContainer.data(), item->at(i), this)));
	}

	qSort(m_items.begin(), m_items.end(), compareByFileNames);
}

IDM_PLUGIN_NS_END
