#include "idmpropertiestable.h"


IDM_PLUGIN_NS_BEGIN

QByteArray PropertiesTable::create()
{
	return "create table PROPERTIES (ID int primary key, ENTITY_ID int, ENTITY_PROPERTY_ID int, NAME char(256))";
}

QByteArray PropertiesTable::select(Database::id_type entity)
{
	return QString::fromLatin1(
			"select ENTITY.*, PROPERTIES.NAME from PROPERTIES"
			"	left join ENTITY on ENTITY.ID = PROPERTIES.ENTITY_PROPERTY_ID"
			"	where PROPERTIES.ENTITY_ID = %1"
			"").arg(QString::number(entity)).toUtf8();
}

IDM_PLUGIN_NS_END
