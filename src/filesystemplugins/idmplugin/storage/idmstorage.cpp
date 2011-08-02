#include "idmstorage.h"


FILE_SYSTEM_NS_BEGIN

IdmStorage::IdmStorage(const QFileInfo &storage) :
	m_valid(true),
	m_db(0),
	m_info(storage)
{
	if (m_info.exists())
	{

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

FILE_SYSTEM_NS_END
