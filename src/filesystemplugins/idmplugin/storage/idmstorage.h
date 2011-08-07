#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include "entities/idmentity.h"
#include "entities/idmentityroot.h"
#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	IdmStorage(const QFileInfo &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	IdmEntity *createEntity(const QString &name, const IdmEntity::Type type);
	void addProperty(IdmEntity *entity, IdmEntity *property);

private:
	IdmEntity::id_type loadId(const QString &tableName) const;
	bool isThereCycles(IdmEntityList *entity, IdmEntity *property) const;
	void loadEntities(sqlite3_stmt *statement, IdmEntityList *parent);

private:
	void setLastError(const char *sqlQuery) const;
	void setLastError(const char *sqlQuery, const char *errorMsg) const;
	void setLastError(const QByteArray &sqlQuery) const;
	void setLastError(const QByteArray &sqlQuery, const char *errorMsg) const;
	void setLastError(const QString &error) const;

private:
	bool m_valid;
	sqlite3 *m_db;
	QFileInfo m_info;
	IdmEntityRoot m_entities;
	mutable QString m_lastError;
};

FILE_SYSTEM_NS_END

#endif /* IDMSTORAGE_H_ */
