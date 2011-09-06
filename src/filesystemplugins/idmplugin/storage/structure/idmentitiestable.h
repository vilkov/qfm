#ifndef IDMENTITIESTABLE_H_
#define IDMENTITIESTABLE_H_

#include "idmdatabasetypes.h"


IDM_PLUGIN_NS_BEGIN

struct EntitiesTable
{
	enum Fields
	{
		Id = 0,
		Type = 1,
		Name = 2,
		ShortFormat = 3
	};

	static QByteArray create();
	static QByteArray select();
	static QByteArray insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat);
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESTABLE_H_ */
