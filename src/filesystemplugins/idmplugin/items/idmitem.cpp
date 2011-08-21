#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

IdmItem::IdmItem(IdmItem *parent) :
	m_parent(parent)
{}

IdmItem::~IdmItem()
{}

FILE_SYSTEM_NS_END
