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

QByteArray PropertiesTable::insert(Database::id_type id, Database::id_type entity, Database::id_type property, const QString &name)
{
	return QString::fromLatin1("insert into PROPERTIES (ID, ENTITY_ID, ENTITY_PROPERTY_ID, NAME) values (%1, %2, %3, '%4');"
							   "create table ENTITY_%2_PROPERTY_%3 (ID int primary key, ENTITY_VALUE_ID int, PROPERTY_VALUE_ID int)").
							   arg(QString::number(id)).
							   arg(QString::number(entity)).
							   arg(QString::number(property)).
							   arg(name).toUtf8();
}

QByteArray PropertiesTable::remove(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("delete from PROPERTIES where ENTITY_ID = %1 and ENTITY_PROPERTY_ID = %2").
			arg(QString::number(entity)).
			arg(QString::number(property)).toUtf8();
}

IDM_PLUGIN_NS_END
