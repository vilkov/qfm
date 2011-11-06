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

	static QString tableName();

	static QByteArray create();
	static QByteArray select();
	static QByteArray insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat);
	static QByteArray remove(Database::id_type entity);
	static QByteArray selectValues(Database::id_type entity, Database::id_type property);
	static QByteArray addValue(Database::id_type entity, Database::id_type id);
	static QByteArray addValue(Database::id_type entity, Database::EntityType type, Database::id_type id, const QVariant &value);
	static QByteArray updateValue(Database::id_type entity, Database::EntityType type, Database::id_type id, const QVariant &value);
	static QByteArray removeValues(Database::id_type entity, const Database::IdsList &ids);
	static QByteArray removeProperty(Database::id_type entity, Database::id_type property);
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESTABLE_H_ */
