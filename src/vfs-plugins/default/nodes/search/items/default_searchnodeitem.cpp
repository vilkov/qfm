#include "default_searchnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchNodeItem::SearchNodeItem(SnapshotItem::Holder &file, Base *parent) :
	NodeItem(file->info(), parent),
	m_file(file.take())
{}

QVariant SearchNodeItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::ToolTipRole)
		return m_file->container()->location().as<QString>();
	else
		return NodeItem::data(column, role);
}

DEFAULT_PLUGIN_NS_END
