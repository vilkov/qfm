#include "idm_entitiestable.h"


IDM_PLUGIN_NS_BEGIN

QString EntitiesTable::tableName()
{
	return QString::fromLatin1("ENTITY");
}

QByteArray EntitiesTable::create()
{
	return "create table ENTITY ("
			"ID int primary key,"
			"TYPE int,"
			"NAME char(256),"
			"SHORT_FORMAT char(256),"
			"EDITOR_GEOMETRY blob(16)," // sizeof(int) * 4
			"LIST_GEOMETRY blob(16))";  // sizeof(int) * 4
}

QByteArray EntitiesTable::select()
{
	return "select * from ENTITY";
}

QByteArray EntitiesTable::select(Database::id_type id)
{
	return QByteArray("select * from ENTITY_").append(QByteArray::number(id));
}

QByteArray EntitiesTable::insert()
{
	return QString::fromLatin1("insert into ENTITY"
			"(ID, TYPE, NAME, SHORT_FORMAT, EDITOR_GEOMETRY, LIST_GEOMETRY)"
			"values"
			"(?1, ?2, ?3, ?4, ?5, ?6)").toUtf8();
}

QByteArray EntitiesTable::insertIntoSingle(Database::id_type id)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID, VALUE) values (?1, ?2)").
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::insertIntoComposite(Database::id_type id)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID) values (?1)").
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::create(Database::EntityType type, Database::id_type id)
{
	if (type == Database::Composite)
		return QString::fromLatin1("create table ENTITY_%1 (ID int primary key)").
								   arg(QString::number(id)).toUtf8();
	else
		if (type == Database::String || type == Database::Path)
			return QString::fromLatin1("create table ENTITY_%1 (ID int primary key, VALUE %2);"
									   "create index ENTITY_%1_INDEX on ENTITY_%1 (VALUE)").
									   arg(QString::number(id)).
									   arg(Database::typeToString(type)).toUtf8();
		else
			return QString::fromLatin1("create table ENTITY_%1 (ID int primary key, VALUE %2)").
									   arg(QString::number(id)).
									   arg(Database::typeToString(type)).toUtf8();
}

QByteArray EntitiesTable::insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat)
{
	if (type == Database::Composite)
		return QString::fromLatin1("insert into ENTITY (ID, TYPE, NAME, SHORT_FORMAT) values (%1, %2, '%3', '%4');"
								   "create table ENTITY_%1 (ID int primary key)").
								   arg(QString::number(id)).
								   arg(QString::number(type)).
								   arg(name).
								   arg(shortFormat).toUtf8();
	else
		if (type == Database::String || type == Database::Path)
			return QString::fromLatin1("insert into ENTITY (ID, TYPE, NAME, SHORT_FORMAT) values (%1, %2, '%3', '%4');"
									   "create table ENTITY_%1 (ID int primary key, VALUE %5);"
									   "create index ENTITY_%1_INDEX on ENTITY_%1 (VALUE)").
									   arg(QString::number(id)).
									   arg(QString::number(type)).
									   arg(name).
									   arg(shortFormat).
									   arg(Database::typeToString(type)).toUtf8();
		else
			return QString::fromLatin1("insert into ENTITY (ID, TYPE, NAME, SHORT_FORMAT) values (%1, %2, '%3', '%4');"
									   "create table ENTITY_%1 (ID int primary key, VALUE %5)").
									   arg(QString::number(id)).
									   arg(QString::number(type)).
									   arg(name).
									   arg(shortFormat).
									   arg(Database::typeToString(type)).toUtf8();
}

QByteArray EntitiesTable::updateEditorGeometry(Database::id_type entity)
{
	return QString::fromLatin1("update ENTITY set EDITOR_GEOMETRY = ?1 where ID = ").
			append(QString::number(entity)).toUtf8();
}

QByteArray EntitiesTable::updateListGeometry(Database::id_type entity)
{
	return QString::fromLatin1("update ENTITY set LIST_GEOMETRY = ?1 where ID = ").
			append(QString::number(entity)).toUtf8();
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

QByteArray EntitiesTable::addCompositeValue(Database::id_type entity, Database::id_type id)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID) values (%2)").
			arg(QString::number(entity)).
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::addValue(Database::id_type entity, Database::id_type id)
{
	return QString::fromLatin1("insert into ENTITY_%1 (ID, VALUE) values (%2, ?1)").
			arg(QString::number(entity)).
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::updateValue(Database::id_type entity, Database::id_type id)
{
	return QString::fromLatin1("update ENTITY_%1 set VALUE = ?1 where ID = %2").
			arg(QString::number(entity)).
			arg(QString::number(id)).toUtf8();
}

QByteArray EntitiesTable::removeValue(Database::id_type entity, Database::id_type value)
{
	return QString::fromLatin1("delete from ENTITY_%1 where ID = %2").
			arg(QString::number(entity)).
			arg(QString::number(value)).toUtf8();
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
