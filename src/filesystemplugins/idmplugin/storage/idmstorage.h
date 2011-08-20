#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>
#include "entities/idmentityroot.h"
#include "../../../filesystem/filesystem_ns.h"
#include "../../../filesystem/info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	typedef IdmEntity::id_type        id_type;
	typedef IdmEntity::size_type      size_type;
	typedef QSet<id_type>             IdsSet;
	typedef QList<id_type>            IdsList;
	typedef QMap<IdmEntity*, IdsList> IdsMap;
	enum { InvalidId = IdmEntity::InvalidId };
	enum { InvalidIndex = IdmEntity::InvalidIndex };

public:
	IdmStorage(const Info &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	IdmEntity *at(size_type index) const { return m_entities.at(index); }
	size_type size() const { return m_entities.size(); }
	size_type indexOf(IdmEntity *item) const { return m_entities.indexOf(item); }
	size_type indexOf(id_type id) const { return m_entities.indexOf(id); }

	bool transaction();
	bool commit();
	void rollback();

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type);
	bool removeEntity(IdmEntity *entity);

	bool addValue(IdmEntity *entity, const IdsMap &values);
	bool addValue(IdmEntity *entity, const QVariant &value);
	bool removeValue(IdmEntity *entity, const IdsList &ids);

	bool addProperty(IdmEntity *entity, IdmEntity *property);
	bool removeProperty(IdmEntity *entity, IdmEntity *property);

private:
	QString idsToString(const IdsSet &ids) const;
	QString idsToString(const IdsList &ids) const;
	QString typeToString(IdmEntity::Type type) const;
	IdmEntity::id_type loadId(const QString &tableName) const;
	bool isThereCycles(IdmEntity *entity, IdmEntity *property) const;
	void loadEntities(sqlite3_stmt *statement, IdmEntity *parent);
	bool removeOverlappingIds(IdmEntity *entity, IdmEntity *property, IdsSet &ids) const;
	bool cleanupParentsValues(IdmEntity *entity, const IdsList &ids);
	bool cleanupPropertyValues(IdmEntity *entity, IdmEntity *property);

private:
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
