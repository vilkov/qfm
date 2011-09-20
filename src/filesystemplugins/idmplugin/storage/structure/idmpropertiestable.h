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

	static QString tableName();

	static QByteArray create();
	static QByteArray select(Database::id_type entity);
	static QByteArray insert(Database::id_type id, Database::id_type entity, Database::id_type property, const QString &name);
	static QByteArray remove(Database::id_type entity, Database::id_type property);

	struct Incomplete
	{
		static QString selectValues(Database::id_type property);
		static QString selectValues(Database::id_type entity, const Database::IdsList &ids);
		static QString dropProperty(Database::id_type property);
		static QString dropProperty2(Database::id_type property);
		static QString removeValues(Database::id_type property, const Database::IdsList &ids);
	};

	struct Parameters
	{
		static QByteArray addValue(Database::id_type entity, Database::id_type property, Database::id_type value);
	};
};

IDM_PLUGIN_NS_END

#endif /* IDMPROPERTIESTABLE_H_ */
