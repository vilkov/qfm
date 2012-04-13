#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/tasks/concrete/containers/filesysteminfolistitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultPathItem(container, value->value().toString(), parent),
	m_node(NULL),
	m_value(value)
{
	lock(tr("Updating..."));
}

QString QueryResultPathValueItem::fileName() const
{
	return m_value->value().toString();
}

void QueryResultPathValueItem::update(const InfoItem *item)
{
	m_info = *item;

	if (item->isDir())
		for (InfoListItem::size_type i = 0, size = static_cast<const InfoListItem *>(item)->size(); i < size; ++i)
			m_items.push_back(new QueryResultPathItem(static_cast<const InfoListItem *>(item)->at(i), this));
}

IDM_PLUGIN_NS_END
