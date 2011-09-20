#include "idmentitiestable.h"


IDM_PLUGIN_NS_BEGIN

QString EntitiesTable::tableName()
{
	return "ENTITY";
}

QByteArray EntitiesTable::create()
{
	return "create table ENTITY (ID int primary key, TYPE int, NAME char(256), SHORT_FORMAT char(256))";
}

QByteArray EntitiesTable::select()
{
	return "select * from ENTITY";
}

QByteArray EntitiesTable::insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat)
{
	return QString::fromLatin1("insert into ENTITY (ID, TYPE, NAME, SHORT_FORMAT) values (%1, %2, '%3', '%4');"
							   "create table ENTITY_%1 (ID int primary key, VALUE %5)").
							   arg(QString::number(id)).
							   arg(QString::number(type)).
							   arg(name).
							   arg(shortFormat).
							   arg(Database::typeToString(type)).toUtf8();
}

QByteArray EntitiesTable::remove(Database::id_type entity)
{
	return QString::fromLatin1("delete from ENTITY where ID = %1;"
							   "delete from PROPERTIES where ENTITY_PROPERTY_ID = %1;"
							   "drop table ENTITY_%1").
							   arg(QString::number(entity)).toUtf8();
}

QByteArray EntitiesTable::selectValues(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_%1_PROPERTY_%2").
			arg(QString::number(entity)).
			arg(QString::number(property)).toUtf8();
}

QByteArray EntitiesTable::addValue(Database::id_type entity, Database::id_type id)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID, VALUE) values (%2, %2)").
			arg(QString::number(entity)).
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::addValue(Database::id_type entity, Database::EntityType type, Database::id_type id, const QVariant &value)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID, VALUE) values (%2, %3)").
			arg(QString::number(entity)).
			arg(QString::number(id)).
			arg(Database::valueToString(type, value)).toUtf8();
}

QByteArray EntitiesTable::removeValues(Database::id_type entity, const Database::IdsList &ids)
{
	return QString::fromLatin1("delete from ENTITY_%1 where ID in (%2)").
			arg(QString::number(entity)).
			arg(Database::idsToString(ids)).toUtf8();
}

QByteArray EntitiesTable::removeProperty(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("drop table ENTITY_%1_PROPERTY_%2").
			arg(QString::number(entity)).
			arg(QString::number(property)).toUtf8();
}

IDM_PLUGIN_NS_END
