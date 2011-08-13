#include "idmstorage.h"
#include "entities/idmentityvalue.h"
#include "../../../tools/pbytearray/pbytearray.h"


FILE_SYSTEM_NS_BEGIN

IdmStorage::IdmStorage(const Info &storage) :
	m_info(storage),
	m_valid(true),
	m_db(0),
	m_entities(0, QString())
{
	if (m_info.exists())
		if (sqlite3_open16(m_info.absoluteFilePath().unicode(), &m_db) == SQLITE_OK)
		{
			sqlite3_stmt *statement;
			const PByteArray sqlQuery("select * from ENTITY");

			if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
			{
				loadEntities(statement, &m_entities);

				if (sqlite3_finalize(statement) != SQLITE_OK)
				{
					m_valid = false;
					setLastError(QString::fromLatin1("Failed to load entities"));
				}
			}
			else
			{
				m_valid = false;
				setLastError(sqlQuery);
			}
		}
		else
		{
			m_valid = false;
			setLastError(QString::fromLatin1("Failed to open DB"));
		}
	else
		if (sqlite3_open16(m_info.absoluteFilePath().unicode(), &m_db) == SQLITE_OK)
		{
			char *error;
			const PByteArray sqlQuery("create table ENTITY (ID int primary key, TYPE int, NAME char(1024));"
									  "create table PROPERTIES (ID int primary key, ENTITY_ID int, ENTITY_PROPERTY_ID int)");

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) != SQLITE_OK)
			{
				m_valid = false;
				setLastError(sqlQuery, error);
			}

			sqlite3_free(error);
		}
		else
		{
			m_valid = false;
			setLastError(QString::fromLatin1("Failed to open DB"));
		}
}

IdmStorage::~IdmStorage()
{
	sqlite3_close(m_db);
}

IdmEntity *IdmStorage::createEntity(const QString &name, IdmEntity::Type type)
{
	IdmEntity *res = 0;
	IdmEntity::id_type id = loadId("ENTITY");

	if (id != IdmEntity::InvalidId)
	{
		char *errorMsg;
		QByteArray sqlQuery = QString::fromLatin1("insert into ENTITY (ID, TYPE, NAME) values (%1, %2, '%3');"
												  "create table ENTITY_%1 (ID int primary key, VALUE %4)").
												  arg(QString::number(id)).
												  arg(QString::number(type)).
												  arg(name).
												  arg(typeToString(type)).toLatin1();

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			if (type == IdmEntity::Composite)
				m_entities.add(res = new IdmEntityList(id, name));
			else
				m_entities.add(res = new IdmEntityValue(type, id, name));
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return res;
}

void IdmStorage::removeEntity(IdmEntity *entity)
{

}

void IdmStorage::addProperty(IdmEntity *entity, IdmEntity *property)
{
	if (entity->id() != property->id() &&
		entity->type() == IdmEntity::Composite &&
		static_cast<IdmEntityList*>(entity)->indexOf(property->id()) == IdmEntityList::InvalidIndex &&
		!isThereCycles(static_cast<IdmEntityList*>(entity), property))
	{
		IdmEntity::id_type id = loadId("PROPERTIES");

		if (id != IdmEntity::InvalidId && transaction())
		{
			char *errorMsg;
			QByteArray sqlQuery = QString::fromLatin1("insert into PROPERTIES (ID, ENTITY_ID, ENTITY_PROPERTY_ID) values (%1, %2, %3);"
													  "create table ENTITY_%2_PROPERTY_%3 (ID int primary key, ENTITY_ID int, PROPERTY_ID int)").
													  arg(QString::number(id)).
													  arg(QString::number(entity->id())).
													  arg(QString::number(property->id())).toLatin1();

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				static_cast<IdmEntityList*>(entity)->add(property);
				property->addParent(entity);
			}
			else
			{
				rollback();
				setLastError(sqlQuery, errorMsg);
			}

			sqlite3_free(errorMsg);
		}
	}
}

void IdmStorage::removeProperty(IdmEntity *entity, IdmEntity *property)
{

}

QString IdmStorage::typeToString(IdmEntity::Type type) const
{
	switch (type)
	{
		case IdmEntity::Composite:
		case IdmEntity::Rating:
		case IdmEntity::Int:      return QString::fromLatin1("int");
		case IdmEntity::String:   return QString::fromLatin1("char(1024)");
		case IdmEntity::Date:     return QString::fromLatin1("date");
		case IdmEntity::Time:     return QString::fromLatin1("time");
		case IdmEntity::DateTime: return QString::fromLatin1("datetime");
		case IdmEntity::Memo:     return QString::fromLatin1("text");
	}
}

IdmEntity::id_type IdmStorage::loadId(const QString &tableName) const
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = QString::fromLatin1("select ID from %1 order by ID desc").arg(tableName).toLatin1();
	IdmEntity::id_type res = IdmEntity::InvalidId;

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		if (sqlite3_step(statement) == SQLITE_ROW)
			res = sqlite3_column_int(statement, 0) + 1;
		else
			res = 1;

		sqlite3_finalize(statement);
	}
	else
		setLastError(sqlQuery);

	return res;
}

bool IdmStorage::isThereCycles(IdmEntityList *entity, IdmEntity *property) const
{
	const IdmEntity::Parents &parents = entity->parents();

	for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
		if (parents.at(i)->id() == property->id())
			return true;
		else
			if (isThereCycles(static_cast<IdmEntityList*>(parents.at(i)), property))
				return true;

	return false;
}

void IdmStorage::loadEntities(sqlite3_stmt *statement, IdmEntityList *parent)
{
	IdmEntity::Type type;
	IdmEntity::id_type id;
	IdmEntityList::size_type index;

	for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
		switch (type = static_cast<IdmEntity::Type>(sqlite3_column_int(statement, 1)))
		{
			case IdmEntity::Int:
			case IdmEntity::String:
			case IdmEntity::Date:
			case IdmEntity::Time:
			case IdmEntity::DateTime:
			case IdmEntity::Memo:
			case IdmEntity::Rating:
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntityList::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index));
						m_entities.at(index)->addParent(parent);
					}
				}
				else
				{
					IdmEntityValue *entity = new IdmEntityValue(type, id, QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)));

					m_entities.add(entity);

					if (&m_entities != parent)
					{
						parent->add(entity);
						entity->addParent(parent);
					}
				}
				break;
			}
			case IdmEntity::Composite:
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntityList::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index));
						m_entities.at(index)->addParent(parent);
					}
				}
				else
				{
					sqlite3_stmt *statement2;
					IdmEntityList *entity = new IdmEntityList(id, QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)));
					QByteArray query = QString::fromLatin1("select * from ENTITY where ID in (select ENTITY_PROPERTY_ID from PROPERTIES where ENTITY_ID = %1)").arg(QString::number(entity->id())).toLatin1();

					m_entities.add(entity);

					if (&m_entities != parent)
					{
						parent->add(entity);
						entity->addParent(parent);
					}

					if (sqlite3_prepare_v2(m_db, query.data(), query.size(), &statement2, NULL) == SQLITE_OK)
					{
						loadEntities(statement2, entity);

						if (sqlite3_finalize(statement2) != SQLITE_OK)
						{
							m_valid = false;
							setLastError(QString::fromLatin1("Failed to load entities"));
						}
					}
					else
					{
						m_valid = false;
						setLastError(query);
					}
				}

				break;
			}
		}
}

bool IdmStorage::transaction()
{
	char *errorMsg;
	const PByteArray sqlQuery("begin transaction");

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		return true;
	else
	{
		setLastError(sqlQuery, errorMsg);
		return false;
	}
}

bool IdmStorage::commit()
{
	char *errorMsg;
	const PByteArray sqlQuery("commit");

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		return true;
	else
	{
		setLastError(sqlQuery, errorMsg);
		return false;
	}
}

void IdmStorage::rollback()
{
	char *errorMsg;
	const PByteArray sqlQuery("rollback");

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) != SQLITE_OK)
		setLastError(sqlQuery, errorMsg);
}

void IdmStorage::setLastError(const char *sqlQuery) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query").arg(QString::fromLatin1(sqlQuery));
}

void IdmStorage::setLastError(const char *sqlQuery, const char *errorMsg) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query (%2)").arg(QString::fromLatin1(sqlQuery)).arg(QString::fromUtf8(errorMsg));
}

void IdmStorage::setLastError(const QByteArray &sqlQuery) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query").arg(QString::fromLatin1(sqlQuery));
}

void IdmStorage::setLastError(const QByteArray &sqlQuery, const char *errorMsg) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query (%2)").arg(QString::fromLatin1(sqlQuery)).arg(QString::fromUtf8(errorMsg));
}

void IdmStorage::setLastError(const QString &error) const
{
	m_lastError = error;
}

FILE_SYSTEM_NS_END
