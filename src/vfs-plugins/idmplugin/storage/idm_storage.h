#ifndef IDM_STORAGE_H_
#define IDM_STORAGE_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>

#include <vfs/location/vfs_location.h>

#include "entities/idm_rootentity.h"
#include "queries/idm_query.h"
#include "queries/idm_querycontext.h"
#include "undo/idm_storage_undocommand.h"
#include "format/idm_shortformat.h"
#include "values/idm_entityvalue.h"
#include "../idm_ns.h"


typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;


IDM_PLUGIN_NS_BEGIN

class Storage
{
	Q_DECLARE_TR_FUNCTIONS(Storage)

public:
	typedef Entity::id_type        id_type;
	typedef Entity::size_type      size_type;
	typedef Database::IdsSet       IdsSet;
	typedef Database::IdsList      IdsList;
	typedef QMap<Entity*, IdsList> IdsMap;
	enum { InvalidId = Entity::InvalidId };
	enum { InvalidIndex = Entity::InvalidIndex };

public:
	Storage(const Location &storage, bool create);
	Storage(const Location &newStorage, const Location &oldStorage);
	~Storage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	Entity *at(size_type index) const { return m_entities.at(index).entity; }
	size_type size() const { return m_entities.size(); }
	size_type indexOf(id_type id) const { return m_entities.indexOf(id); }

	bool transaction();
	bool commit();
	void rollback();

	bool savepoint(const QByteArray &name);
	bool release(const QByteArray &name);
	void rollback(const QByteArray &name);

	QueryContext prepare(const Query &query, QString &error) const;

	Entity *createEntity(const QString &name, Entity::Type type, const ShortFormat &shortFormat);
	bool updateEditorGeometry(Entity *entity, const QRect &geometry);
	bool updateListGeometry(Entity *entity, const QRect &geometry);
	bool removeEntity(Entity *entity);

	bool addProperty(Entity *entity, Entity *property, const QString &name);
	bool renameProperty(Entity *entity, Entity *property, const QString &name);
	bool removeProperty(Entity *entity, Entity *property);

	EntityValue::Holder addValue(Entity *entity) const;
	bool addValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const;
	bool addValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues) const;
	EntityValue::Holder addValue(Entity *entity, const QVariant &value) const;
	bool updateValue(const EntityValue::Holder &value, const QVariant &newValue) const;
	bool removeValue(Entity *entity, const IdsList &ids) const;
	bool removeValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const;

private:
	id_type loadId(const QString &tableName) const;
	bool isThereCycles(Entity *entity, Entity *property) const;
	bool removeEntityValue(Entity *entity, id_type id) const;
	bool removeEntityValues(Entity *entity, const IdsList &ids) const;
	bool removeOverlappingIds(Entity *entity, Entity *property, IdsSet &ids) const;
	bool removeSelfOverlappingIds(Entity *entity, const IdsList &entityIds, Entity *property, IdsSet &propertyIds) const;
	bool cleanupParentsValues(Entity *entity) const;
	bool cleanupParentsValues(Entity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(Entity *entity) const;
	bool cleanupPropertyValues(Entity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(Entity *entity, Entity *property) const;

	void loadEntities();
	void loadProperties();
	void loadEntities(sqlite3_stmt *statement, Entity *parent);
	void copyEntities(sqlite3 *oldDb, QByteArray &sqlQuery);
	void copyProperties(sqlite3 *oldDb, QByteArray &sqlQuery);
	void copySingleEntityValues(sqlite3 *oldDb, Entity *entity, QByteArray &sqlQuery);
	void copyCompositeEntityValues(sqlite3 *oldDb, Entity *entity, QByteArray &sqlQuery);
	void copyPropertyValues(sqlite3 *oldDb, Entity *entity, Entity *property, QByteArray &sqlQuery);

private:
	void performUndo();
	void performUndo(const QByteArray &name);
	void clearUndoStack();
	void clearUndoStack(const QByteArray &name);

private:
	QString failedToLoadId(const QString &tableName) const;
	QString failedToBind(const QByteArray &sqlQuery) const;
	QString failedToReset(const QByteArray &sqlQuery) const;

	void setLastError(const QByteArray &sqlQuery, sqlite3 *db) const;
	void setLastError(const char *sqlQuery) const;
	void setLastError(const char *sqlQuery, const char *errorMsg) const;
	void setLastError(const QByteArray &sqlQuery) const;
	void setLastError(const QByteArray &sqlQuery, const char *errorMsg) const;
	void setLastError(const QString &error) const;

private:
	typedef QList<StorageUndoCommand*>                         UndoList;
	typedef ::Tools::Containers::HashedList<QByteArray, UndoList> UndoStack;

private:
	QMutex m_mutex;
	bool m_valid;
	sqlite3 *m_db;
	mutable UndoStack m_undo;
	RootEntity m_entities;
	mutable QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_H_ */
