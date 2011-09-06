#ifndef IDMPROPERTIESTABLE_H_
#define IDMPROPERTIESTABLE_H_

#include "idmdatabasetypes.h"


IDM_PLUGIN_NS_BEGIN

struct PropertiesTable
{
	enum Fields
	{
		Id = 0,
		EntityId = 1,
		EntityPropertyId = 2,
		Name = 3
	};

	static QByteArray create();
	static QByteArray select(Database::id_type entity);
};

IDM_PLUGIN_NS_END

#endif /* IDMPROPERTIESTABLE_H_ */
