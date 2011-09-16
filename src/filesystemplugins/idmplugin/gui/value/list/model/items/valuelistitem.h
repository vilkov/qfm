#ifndef VALUELISTITEM_H_
#define VALUELISTITEM_H_

#include <QtCore/QVariant>
#include "../../../../../storage/structure/idmdatabasetypes.h"


IDM_PLUGIN_NS_BEGIN

class ValueListItem
{
public:
	ValueListItem(Database::id_type id, const QVariant &value) :
		m_id(id),
		m_value(value)
	{}

	Database::id_type id() const { return m_id; }
	const QVariant &value() const { return m_value; }

private:
	Database::id_type m_id;
	QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTITEM_H_ */
