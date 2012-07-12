#include "m3uitem.h"


M3U_PLUGIN_NS_BEGIN

M3uItem::M3uItem(M3uItem *parent) :
	Item(parent)
{}

bool M3uItem::isList() const
{
	return false;
}

M3U_PLUGIN_NS_END
