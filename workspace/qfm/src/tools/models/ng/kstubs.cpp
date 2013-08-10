#include "kstubs.h"
#include <QtCore/QHash>


uint qHash(const KFileItem &item)
{
	return qHash(item.m_string);
}
