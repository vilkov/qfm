#include "idmentitiestable.h"


IDM_PLUGIN_NS_BEGIN

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

IDM_PLUGIN_NS_END
