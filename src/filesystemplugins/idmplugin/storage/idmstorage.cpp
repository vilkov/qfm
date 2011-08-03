#include "idmstorage.h"
#include "entities/idmentityvalue.h"
#include "../../../tools/pbytearray/pbytearray.h"


FILE_SYSTEM_NS_BEGIN

IdmStorage::IdmStorage(const QFileInfo &storage) :
	m_valid(true),
	m_db(0),
	m_info(storage),
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
					m_lastError = QString::fromUtf8("Failed to load entities");
				}
			}
			else
			{
				m_valid = false;
				m_lastError = QString::fromUtf8("Failed to load entities");
			}
		}
		else
		{
			m_valid = false;
			m_lastError = tr("Failed to open DB");
		}
	else
		if (sqlite3_open16(m_info.absoluteFilePath().unicode(), &m_db) == SQLITE_OK)
		{
			char *error;

			if (sqlite3_exec(m_db,
							 "create table ENTITY (ID int primary key, TYPE int, NAME char(1024));"
							 "create table PROPERTIES (ID int primary key, ENTITY_ID int, ENTITY_PROPERTY_ID int)",
							 NULL,
							 NULL,
							 &error) != SQLITE_OK)
			{
				m_valid = false;
				m_lastError = QString::fromUtf8(error);
			}

			sqlite3_free(error);
		}
		else
		{
			m_valid = false;
			m_lastError = tr("Failed to open DB");
		}
}

IdmStorage::~IdmStorage()
{
	sqlite3_close(m_db);
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
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntityList::InvalidIndex)
				{
					if (&m_entities != parent)
						parent->add(m_entities.at(index));
				}
				else
				{
					IdmEntityValue *entity = new IdmEntityValue(type, id, QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)));

					m_entities.add(entity);

					if (&m_entities != parent)
						parent->add(entity);
				}
				break;
			}
			case IdmEntity::Composite:
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntityList::InvalidIndex)
				{
					if (&m_entities != parent)
						parent->add(m_entities.at(index));
				}
				else
				{
					sqlite3_stmt *statement2;
					PByteArray sqlQuery("select * from ENTITY where ID in (select ENTITY_PROPERTY_ID from PROPERTIES where ENTITY_ID = %1)");
					IdmEntityList *entity = new IdmEntityList(id, QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)));
					QByteArray query = QString::fromLatin1(sqlQuery.data(), sqlQuery.size()).arg(QString::number(entity->id())).toLatin1();

					m_entities.add(entity);

					if (&m_entities != parent)
						parent->add(entity);

					if (sqlite3_prepare_v2(m_db, query.data(), query.size(), &statement2, NULL) == SQLITE_OK)
					{
						loadEntities(statement2, entity);

						if (sqlite3_finalize(statement2) != SQLITE_OK)
						{
							m_valid = false;
							m_lastError = QString::fromUtf8("Failed to load entities");
						}
					}
					else
					{
						m_valid = false;
						m_lastError = QString::fromUtf8("Failed to load entities");
					}
				}

				break;
			}
			case IdmEntity::Raiting:
			{
				break;
			}
		}
}

FILE_SYSTEM_NS_END
