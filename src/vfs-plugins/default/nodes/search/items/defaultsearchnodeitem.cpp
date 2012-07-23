#include "defaultsearchnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchNodeItem::SearchNodeItem(SnapshotItem::Holder &file, Base *parent) :
	NodeItem(file->info(), parent),
	m_file(file.take())
{}

DEFAULT_PLUGIN_NS_END
