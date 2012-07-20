#include "idmqueryresultrootpathvalueitem.h"
#include "idmqueryresultpathvalueitem.h"
#include "../../../interfaces/idminvalidfileinfo.h"
#include "../../../../../application.h"

#include <vfs/model/vfs_proxymodel.h>


IDM_PLUGIN_NS_BEGIN

inline static bool compareByFileNames(const Item::Holder &v1, const Item::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultPathItem>()->info(), v2.as<QueryResultPathItem>()->info());
}


QueryResultRootPathValueItem::QueryResultRootPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
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
		Application::desktopService()->open(folder, info());
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
