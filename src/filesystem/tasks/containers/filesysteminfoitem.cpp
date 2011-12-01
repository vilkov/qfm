#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

InfoItem::InfoItem(const Info &info) :
	Info(info),
	m_shouldRemove(true)
{}

FILE_SYSTEM_NS_END
