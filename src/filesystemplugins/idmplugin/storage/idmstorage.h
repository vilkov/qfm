#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QList>
#include <QtCore/QCoreApplication>
#include "entities/idmentityroot.h"
#include "../../../filesystem/filesystem_ns.h"
#include "../../../filesystem/info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	IdmStorage(const Info &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type);
	void removeEntity(IdmEntity *entity);

	void addProperty(IdmEntity *entity, IdmEntity *property);
	void removeProperty(IdmEntity *entity, IdmEntity *property);

private:
	QString typeToString(IdmEntity::Type type) const;
	IdmEntity::id_type loadId(const QString &tableName) const;
	bool isThereCycles(IdmEntity *entity, IdmEntity *property) const;
	void loadEntities(sqlite3_stmt *statement, IdmEntity *parent);

private:
	bool transaction();
	bool commit();
	void rollback();

	void setLastError(const char *sqlQuery) const;
	void setLastError(const char *sqlQuery, const char *errorMsg) const;
	void setLastError(const QByteArray &sqlQuery) const;
	void setLastError(const QByteArray &sqlQuery, const char *errorMsg) const;
	void setLastError(const QString &error) const;

private:
	Info m_info;
	bool m_valid;
	sqlite3 *m_db;
	IdmEntityRoot m_entities;
	mutable QString m_lastError;
};

FILE_SYSTEM_NS_END

#endif /* IDMSTORAGE_H_ */
