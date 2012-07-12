#include "idmpropertiestable.h"


IDM_PLUGIN_NS_BEGIN

QString PropertiesTable::tableName()
{
	return "PROPERTIES";
}

QByteArray PropertiesTable::create()
{
	return "create table PROPERTIES (ID int primary key, ENTITY_ID int, ENTITY_PROPERTY_ID int, NAME char(256))";
}

QByteArray PropertiesTable::create(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("create table ENTITY_%1_PROPERTY_%2 (ID int primary key, ENTITY_VALUE_ID int, PROPERTY_VALUE_ID int)").
							   arg(QString::number(entity)).
							   arg(QString::number(property)).toUtf8();
}

QByteArray PropertiesTable::select()
{
	return "select * from PROPERTIES";
}

QByteArray PropertiesTable::select(Database::id_type entity)
{
	return QString::fromLatin1(
			"select ENTITY.*, PROPERTIES.NAME from PROPERTIES"
			"	left join ENTITY on ENTITY.ID = PROPERTIES.ENTITY_PROPERTY_ID"
			"	where PROPERTIES.ENTITY_ID = %1"
			"").arg(QString::number(entity)).toUtf8();
}

QByteArray PropertiesTable::select(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("select * from ENTITY_%1_PROPERTY_%2").
							   arg(QString::number(entity)).
							   arg(QString::number(property)).toUtf8();
}

QByteArray PropertiesTable::insert()
{
	return "insert into PROPERTIES (ID, ENTITY_ID, ENTITY_PROPERTY_ID, NAME) values (?1, ?2, ?3, ?4)";
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

QByteArray PropertiesTable::insert(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("insert into ENTITY_%1_PROPERTY_%2"
								"(ID, ENTITY_VALUE_ID, PROPERTY_VALUE_ID)"
								"values"
								"(?1, ?2, ?3)").
							   arg(QString::number(entity)).
							   arg(QString::number(property)).toUtf8();
}

QByteArray PropertiesTable::rename(Database::id_type entity, Database::id_type property, const QString &name)
{
	return QString::fromLatin1("update PROPERTIES set NAME = '").
							   append(name).
							   append(QString::fromLatin1("' where ENTITY_ID = ")).
							   append(QString::number(entity)).
							   append(QString::fromLatin1(" and ENTITY_PROPERTY_ID = ")).
							   append(QString::number(property)).toUtf8();
}

QByteArray PropertiesTable::remove(Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("delete from PROPERTIES where ENTITY_ID = %1 and ENTITY_PROPERTY_ID = %2").
			arg(QString::number(entity)).
			arg(QString::number(property)).toUtf8();
}

QByteArray PropertiesTable::addValue(const QString &tableName, Database::id_type id, Database::id_type entity, Database::id_type property)
{
	return QString::fromLatin1("insert into ").
			append(tableName).
			append(QString::fromLatin1(" (ID, ENTITY_VALUE_ID, PROPERTY_VALUE_ID) values (")).
			append(QString::number(id)).
			append(QChar(',')).
			append(QString::number(entity)).
			append(QChar(',')).
			append(QString::number(property)).
			append(QChar(')')).toUtf8();
}

QByteArray PropertiesTable::removeValue(Database::id_type entity, Database::id_type entityValue, Database::id_type property, Database::id_type propertyValue)
{
	return QByteArray("delete from ENTITY_").
			append(QByteArray::number(entity)).
			append("_PROPERTY_").
			append(QByteArray::number(property)).
			append(" where ENTITY_VALUE_ID = ").
			append(QByteArray::number(entityValue)).
			append(" and PROPERTY_VALUE_ID = ").
			append(QByteArray::number(propertyValue));
}

QByteArray PropertiesTable::selectValuesExcept(Database::id_type entity, const Database::IdsList &entityIds, Database::id_type property, const Database::IdsSet &propertyIds)
{
	return QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_").
			append(QString::number(entity)).
			append(QString::fromLatin1("_PROPERTY_")).
			append(QString::number(property)).
			append(QString::fromLatin1(" where ENTITY_VALUE_ID not in (%1) and PROPERTY_VALUE_ID in (%2)")).
			arg(Database::idsToString(entityIds)).
			arg(Database::idsToString(propertyIds)).toUtf8();
}

QString PropertiesTable::Incomplete::selectValues(Database::id_type property)
{
	return QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_%1_PROPERTY_").
			append(QString::number(property)).
			append(QString::fromLatin1(" where PROPERTY_VALUE_ID in (%2)"));
}

QString PropertiesTable::Incomplete::selectValues(Database::id_type entity, const Database::IdsList &ids)
{
	return QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_").
			append(QString::number(entity)).
			append(QString::fromLatin1("_PROPERTY_%1")).
			append(QString::fromLatin1(" where ENTITY_VALUE_ID in (%1)").arg(Database::idsToString(ids)));
}

QString PropertiesTable::Incomplete::dropProperty(Database::id_type property)
{
	return QString::fromLatin1("drop table ENTITY_%1_PROPERTY_").append(QString::number(property));
}

QString PropertiesTable::Incomplete::dropProperty2(Database::id_type property)
{
	return QString::fromLatin1("drop table ENTITY_%1_PROPERTY_").arg(QString::number(property));
}

QByteArray PropertiesTable::Incomplete::addValue(const QString &tableName, Database::id_type entity)
{
	return QString::fromLatin1("insert into ").
			append(tableName).
			append(QString::fromLatin1(" (ID, ENTITY_VALUE_ID, PROPERTY_VALUE_ID) values (?1, ")).
			append(QString::number(entity)).
			append(QString::fromLatin1(", ?2)")).toUtf8();
}

QString PropertiesTable::Incomplete::removeValues(Database::id_type property, const Database::IdsList &ids)
{
	return QString::fromLatin1("delete from ENTITY_%1_PROPERTY_").
			append(QString::number(property)).
			append(QString::fromLatin1(" where PROPERTY_VALUE_ID in (%1)").arg(Database::idsToString(ids)));
}

QByteArray PropertiesTable::Parameters::addValue(Database::id_type entity, Database::id_type property, Database::id_type value)
{
	return QString::fromLatin1("insert into ENTITY_%1_PROPERTY_%2 (ID, ENTITY_VALUE_ID, PROPERTY_VALUE_ID) values (?, %3, ?)").
			arg(QString::number(entity)).
			arg(QString::number(property)).
			arg(QString::number(value)).toUtf8();
}

IDM_PLUGIN_NS_END
