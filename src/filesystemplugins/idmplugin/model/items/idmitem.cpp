#include "idmitem.h"


IDM_PLUGIN_NS_BEGIN

IdmItem::IdmItem(IdmItem *parent) :
	m_parent(parent)
{}

IdmItem::~IdmItem()
{}

IDM_PLUGIN_NS_END
