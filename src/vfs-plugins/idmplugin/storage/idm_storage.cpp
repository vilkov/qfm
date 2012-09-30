#include "idm_storage.h"
#include "values/idm_valuereader.h"
#include "structure/idm_entitiestable.h"
#include "structure/idm_propertiestable.h"
#include "undo/concrete/idm_storage_undoaddentity.h"
#include "undo/concrete/idm_storage_undoaddproperty.h"
#include "undo/concrete/idm_storage_undoremoveentity.h"
#include "undo/concrete/idm_storage_undoremoveproperty.h"
#include "undo/concrete/idm_storage_undoupdatevalue.h"
#include "undo/concrete/idm_storage_undoaddvalue.h"
#include "undo/concrete/idm_storage_undoremovevalue.h"
#include "undo/concrete/idm_storage_undorenameproperty.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <sqlite3.h>


IDM_PLUGIN_NS_BEGIN

Storage::Storage(const Location &storage, bool create) :
	m_valid(true),
	m_db(0)
{
	if (create)
		if (sqlite3_open16(storage.as<QString>().unicode(), &m_db) == SQLITE_OK)
		{
			char *error;
			QByteArray sqlQuery = Database::init();

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) != SQLITE_OK)
			{
				setLastError(sqlQuery.data(), error);
				m_valid = false;
			}

			sqlite3_free(error);
		}
		else
		{
			setLastError(QString::fromLatin1("Failed to open DB"));
			m_valid = false;
		}
	else
		if (sqlite3_open16(storage.as<QString>().unicode(), &m_db) == SQLITE_OK)
		{
			loadEntities();

			if (m_valid)
				loadProperties();
		}
		else
		{
			setLastError(QString::fromLatin1("Failed to open DB"));
			m_valid = false;
		}
}

Storage::Storage(const Location &newStorage, const Location &oldStorage)
{
	sqlite3 *oldDb;

	if (sqlite3_open16(oldStorage.as<QString>().unicode(), &oldDb) == SQLITE_OK)
	{
		if (sqlite3_open16(newStorage.as<QString>().unicode(), &m_db) == SQLITE_OK)
		{
			char *error;
			QByteArray sqlQuery = Database::init();

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) == SQLITE_OK)
			{
				Entity *entity;
				Entity *property;

				copyEntities(oldDb, sqlQuery);

				if (m_valid)
				{
					copyProperties(oldDb, sqlQuery);

					for (Entity::size_type i = 0, size = m_entities.size(); i < size && m_valid; ++i)
					{
						entity = m_entities.at(i).entity;
						sqlQuery = EntitiesTable::create(entity->type(), entity->id());

						if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) == SQLITE_OK)
							if (entity->type() == Database::Composite)
							{
								copyCompositeEntityValues(oldDb, entity, sqlQuery);

								for (Entity::size_type i = 0, size = entity->size(); i < size && m_valid; ++i)
								{
									property = entity->at(i).entity;
									sqlQuery = PropertiesTable::create(entity->id(), property->id());

									if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) == SQLITE_OK)
										copyPropertyValues(oldDb, entity, property, sqlQuery);
									else
									{
										setLastError(QString::fromUtf8(error));
										sqlite3_free(error);
										m_valid = false;
										break;
									}
								}
							}
							else
								copySingleEntityValues(oldDb, entity, sqlQuery);
						else
						{
							setLastError(QString::fromUtf8(error));
							sqlite3_free(error);
							m_valid = false;
							break;
						}
					}
				}
			}
			else
			{
				setLastError(QString::fromUtf8(error));
				sqlite3_free(error);
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			m_valid = false;
		}

		sqlite3_close(oldDb);
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

Storage::~Storage()
{
	sqlite3_close(m_db);
}

bool Storage::transaction()
{
	if (!m_undo.isEmpty())
		setLastError(tr("There is uncommitted transactions or savepoints!"));
	else
	{
		QByteArray name("transaction");

		if (m_undo.contains(name))
			setLastError(tr("Nested transactions is not supported! Use save points instead."));
		else
		{
			char *errorMsg = 0;
			static const char *sqlQuery = "begin transaction";

			if (sqlite3_exec(m_db, sqlQuery, NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				m_undo.add(name, UndoList());
				return true;
			}
			else
				setLastError(sqlQuery, errorMsg);

			sqlite3_free(errorMsg);
		}
	}

	return false;
}

bool Storage::commit()
{
	char *errorMsg = 0;
	static const char *sqlQuery = "commit";

	if (sqlite3_exec(m_db, sqlQuery, NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		clearUndoStack();
		return true;
	}
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

void Storage::rollback()
{
	char *errorMsg = 0;
	static const char *sqlQuery = "rollback";

	if (sqlite3_exec(m_db, sqlQuery, NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo();
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
}

bool Storage::savepoint(const QByteArray &name)
{
	if (m_undo.contains(name))
		setLastError(tr("Savepoint \"%1\" already exists!").arg(QString::fromLatin1(name)));
	else
	{
		char *errorMsg = 0;
		QByteArray sqlQuery("savepoint '");
		sqlQuery.append(name).append('\'');

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			m_undo.add(name, UndoList());
			return true;
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return false;
}

bool Storage::release(const QByteArray &name)
{
	char *errorMsg = 0;
	QByteArray sqlQuery("release '");
	sqlQuery.append(name).append('\'');

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		clearUndoStack(name);
		return true;
	}
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

void Storage::rollback(const QByteArray &name)
{
	char *errorMsg = 0;
	QByteArray sqlQuery("rollback to '");
	sqlQuery.append(name).append('\'');

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo(name);
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
}

QueryContext Storage::prepare(const Query &query, QString &error) const
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = query.compile();

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		return QueryContext(query.entity(), statement);
	else
	{
		error = QString::fromLatin1("Failed to prepare \"%1\" query").arg(QString::fromLatin1(sqlQuery));
		return QueryContext();
	}
}

Entity *Storage::createEntity(const QString &name, Entity::Type type, const ShortFormat &shortFormat)
{
	Entity *res = 0;
	Entity::id_type id = loadId(QString::fromLatin1("ENTITY"));

	if (id != Entity::InvalidId)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = EntitiesTable::insert(type, id, name, shortFormat.format());

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			m_entities.add(res = new Entity(type, id, name, shortFormat, QByteArray(), QByteArray()), name);
			m_undo.last().push_back(new StorageUndoAddEntity(res));
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return res;
}

bool Storage::updateEditorGeometry(Entity *entity, const QRect &geometry)
{
	if (entity->editorGeometry() == geometry)
		return true;
	else
	{
		bool res = false;
		sqlite3_stmt *statement;
		QByteArray sqlQuery = EntitiesTable::updateEditorGeometry(entity->id());

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			QByteArray buffer = Entity::geometryToByteArray(geometry);

			if (sqlite3_bind_blob(statement, 1, buffer.data(), buffer.size(), SQLITE_STATIC) == SQLITE_OK)
				if (sqlite3_step(statement) == SQLITE_DONE)
				{
					entity->setEditorGeometry(geometry);
					res = true;
				}
				else
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			else
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

			sqlite3_finalize(statement);
		}
		else
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

		return res;
	}
}

bool Storage::updateListGeometry(Entity *entity, const QRect &geometry)
{
	if (entity->listGeometry() == geometry)
		return true;
	else
	{
		bool res = false;
		sqlite3_stmt *statement;
		QByteArray sqlQuery = EntitiesTable::updateListGeometry(entity->id());

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			QByteArray buffer = Entity::geometryToByteArray(geometry);

			if (sqlite3_bind_blob(statement, 1, buffer.data(), buffer.size(), SQLITE_STATIC) == SQLITE_OK)
				if (sqlite3_step(statement) == SQLITE_DONE)
				{
					entity->setListGeometry(geometry);
					res = true;
				}
				else
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			else
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

			sqlite3_finalize(statement);
		}
		else
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

		return res;
	}
}

bool Storage::removeEntity(Entity *entity)
{
	char *errorMsg = 0;
	QByteArray sqlQuery = EntitiesTable::remove(entity->id());

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		if (cleanupParentsValues(entity) &&
			(entity->type() != Database::Composite || cleanupPropertyValues(entity)))
		{
			Entity *parent;
			PScopedPointer<StorageUndoRemoveEntity> command(new StorageUndoRemoveEntity(entity));
			const Entity::Parents &parents = entity->parents();

			for (Entity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
			{
				parent = parents.at(i);
				command->addParent(parent, parent->at(parent->indexOf(entity)).name);
				parents.at(i)->remove(entity);
			}

			for (Entity::size_type i = 0, size = entity->size(); i < size; ++i)
				entity->at(i).entity->removeParent(entity);

			m_entities.remove(entity);
			m_undo.last().push_back(command.take());

			return true;
		}
	}
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

bool Storage::addProperty(Entity *entity, Entity *property, const QString &name)
{
	if (entity->id() != property->id() &&
		entity->type() == Database::Composite &&
		entity->indexOf(property->id()) == Entity::InvalidIndex &&
		!isThereCycles(entity, property))
	{
		Entity::id_type id = loadId(PropertiesTable::tableName());

		if (id != Entity::InvalidId)
		{
			char *errorMsg = 0;
			QByteArray sqlQuery = PropertiesTable::insert(id, entity->id(), property->id(), name);

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				entity->add(property, name);
				property->addParent(entity);
				m_undo.last().push_back(new StorageUndoAddProperty(entity, property));

				return true;
			}
			else
				setLastError(sqlQuery, errorMsg);

			sqlite3_free(errorMsg);
		}
	}

	return false;
}

bool Storage::renameProperty(Entity *entity, Entity *property, const QString &name)
{
	if (entity->indexOf(property->id()) != Entity::InvalidIndex)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = PropertiesTable::rename(entity->id(), property->id(), name);

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			m_undo.last().push_back(new StorageUndoRenameProperty(entity, property));
			entity->rename(property, name);

			return true;
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return false;
}

bool Storage::removeProperty(Entity *entity, Entity *property)
{
	if (entity->id() != property->id() &&
		entity->type() == Database::Composite &&
		entity->indexOf(property->id()) != Entity::InvalidIndex)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = PropertiesTable::remove(entity->id(), property->id());

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			if (cleanupPropertyValues(entity, property))
			{
				sqlQuery = EntitiesTable::removeProperty(entity->id(), property->id());

				if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
				{
					PScopedPointer<StorageUndoRemoveProperty> command(new StorageUndoRemoveProperty(entity, property, entity->at(entity->indexOf(property)).name));
					entity->remove(property);
					property->removeParent(entity);
					m_undo.last().push_back(command.take());

					return true;
				}
				else
					setLastError(sqlQuery, errorMsg);
			}
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return false;
}

EntityValue::Holder Storage::addValue(Entity *entity) const
{
	QString str;
	id_type id = loadId(str = QString::fromLatin1("ENTITY_").append(QString::number(entity->id())));

	if (id != Entity::InvalidId)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = EntitiesTable::addCompositeValue(entity->id(), id);

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			return ValueReader::createValue(entity, id);
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}
	else
		setLastError(failedToLoadId(str));

	return EntityValue::Holder();
}

bool Storage::addValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const
{
	if (entityValue.as<CompositeEntityValue>()->contains(propertyValue))
		setLastError(tr("Entity \"%1\" [\"%2\"] already contains (\"%3\") value.").
				arg(entityValue->entity()->name()).
				arg(entityValue->value().toString()).
				arg(propertyValue->value().toString()));
	else
	{
		QString table =
				QString::fromLatin1("ENTITY_").
				append(QString::number(entityValue->entity()->id())).
				append(QString::fromLatin1("_PROPERTY_")).
				append(QString::number(propertyValue->entity()->id()));
		id_type id = loadId(table);

		if (id != Entity::InvalidId)
		{
			char *errorMsg = 0;
			QByteArray sqlQuery = PropertiesTable::addValue(table, id, entityValue->id(), propertyValue->id());

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				m_undo.last().push_back(new StorageUndoAddValue(entityValue, propertyValue));
				ValueReader::addValue(entityValue, propertyValue);

				return true;
			}
			else
				setLastError(sqlQuery, errorMsg);

			sqlite3_free(errorMsg);
		}
		else
			setLastError(failedToLoadId(table));
	}

	return false;
}

bool Storage::addValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues) const
{
	EntityValue::Holder propertyValue;

	if (entityValue.as<CompositeEntityValue>()->contains(propertyValues, propertyValue))
		setLastError(tr("Entity \"%1\" [\"%2\"] already contains (\"%3\") value.").
				arg(entityValue->entity()->name()).
				arg(entityValue->value().toString()).
				arg(propertyValue->value().toString()));
	else
		if (propertyValues.isEmpty())
			setLastError("Values list is empty!");
		else
		{
			QString table =
					QString::fromLatin1("ENTITY_").
					append(QString::number(entityValue->entity()->id())).
					append(QString::fromLatin1("_PROPERTY_")).
					append(QString::number(propertyValues.at(0)->entity()->id()));
			id_type id = loadId(table);

			if (id != Entity::InvalidId)
			{
				bool ok = true;
				sqlite3_stmt *statement;
				QByteArray sqlQuery = PropertiesTable::Incomplete::addValue(table, entityValue->id());

				if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
				{
					for (CompositeEntityValue::List::size_type i = 0, size = propertyValues.size(); i < size; ++i)
					{
						if (sqlite3_bind_int(statement, 1, id) != SQLITE_OK)
						{
							setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
							ok = false;
							break;
						}

						if (sqlite3_bind_int(statement, 2, propertyValues.at(i)->id()) != SQLITE_OK)
						{
							setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
							ok = false;
							break;
						}

						if (sqlite3_step(statement) == SQLITE_DONE)
							if (sqlite3_reset(statement) == SQLITE_OK)
								++id;
							else
							{
								setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
								ok = false;
								break;
							}
						else
						{
							setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
							ok = false;
							break;
						}
					}

					sqlite3_finalize(statement);
				}
				else
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

				if (ok)
				{
					m_undo.last().push_back(new StorageUndoAddValue(entityValue, propertyValues));
					ValueReader::addValue(entityValue, propertyValues);

					return true;
				}
				else
					return false;
			}
			else
				setLastError(failedToLoadId(table));
		}

	return false;
}

EntityValue::Holder Storage::addValue(Entity *entity, const QVariant &value) const
{
	QString str;
	id_type id = loadId(str = QString::fromLatin1("ENTITY_").append(QString::number(entity->id())));
	EntityValue::Holder res;

	if (id != InvalidId)
	{
		sqlite3_stmt *statement;
		QByteArray sqlQuery = EntitiesTable::addValue(entity->id(), id);

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			QByteArray valueString = Database::valueToString(entity->type(), value).toUtf8();

			if (sqlite3_bind_text(statement, 1, valueString, valueString.size(), SQLITE_STATIC) != SQLITE_OK)
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			else
				if (sqlite3_step(statement) == SQLITE_DONE)
					res = ValueReader::createValue(entity, id, value);
				else
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

			sqlite3_finalize(statement);
		}
		else
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
	}
	else
		setLastError(failedToLoadId(str));

	return res;
}

bool Storage::updateValue(const EntityValue::Holder &value, const QVariant &newValue) const
{
	bool res = false;
	sqlite3_stmt *statement;
	QByteArray sqlQuery = EntitiesTable::updateValue(value->entity()->id(), value->id());

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		QByteArray valueString = Database::valueToString(value->entity()->type(), newValue).toUtf8();

		if (sqlite3_bind_text(statement, 1, valueString, valueString.size(), SQLITE_STATIC) != SQLITE_OK)
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
		else
			if (sqlite3_step(statement) == SQLITE_DONE)
			{
				m_undo.last().push_back(new StorageUndoUpdateValue(value));
				ValueReader::updateValue(value, newValue);
				res = true;
			}
			else
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

		sqlite3_finalize(statement);
	}
	else
		setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));

	return res;
}

bool Storage::removeValue(Entity *entity, const IdsList &ids) const
{
	if (ids.isEmpty())
		return true;
	else
		if (cleanupParentsValues(entity, ids))
			if (entity->type() != Database::Composite)
				return removeEntityValues(entity, ids);
			else
				if (cleanupPropertyValues(entity, ids))
					return removeEntityValues(entity, ids);

	return false;
}

bool Storage::removeValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const
{
	char *errorMsg = 0;
	QByteArray sqlQuery = PropertiesTable::removeValue(entityValue->entity()->id(), entityValue->id(), propertyValue->entity()->id(), propertyValue->id());

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		m_undo.last().push_back(new StorageUndoRemoveValue(entityValue, propertyValue));
		ValueReader::takeValue(entityValue, propertyValue);

		return true;
	}
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

Storage::id_type Storage::loadId(const QString &tableName) const
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = Database::loadId(tableName);
	id_type res = Entity::InvalidId;

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		if (sqlite3_step(statement) == SQLITE_ROW)
			res = sqlite3_column_int(statement, 0) + 1;
		else
			res = 1;

		sqlite3_finalize(statement);
	}

	return res;
}

bool Storage::isThereCycles(Entity *entity, Entity *property) const
{
	const Entity::Parents &parents = entity->parents();

	for (Entity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
		if (parents.at(i)->id() == property->id())
			return true;
		else
			if (isThereCycles(parents.at(i), property))
				return true;

	return false;
}

bool Storage::removeEntityValue(Entity *entity, id_type id) const
{
	char *errorMsg = 0;
	QByteArray sqlQuery = EntitiesTable::removeValue(entity->id(), id);

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		return true;
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

bool Storage::removeEntityValues(Entity *entity, const IdsList &ids) const
{
	char *errorMsg = 0;
	QByteArray sqlQuery = EntitiesTable::removeValues(entity->id(), ids);

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		return true;
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);
	return false;
}

bool Storage::removeOverlappingIds(Entity *entity, Entity *property, IdsSet &ids) const
{
	if (!ids.isEmpty())
	{
		QByteArray sqlQuery;
		sqlite3_stmt *statement;
		const Entity::Parents &parents = property->parents();
		QString query = PropertiesTable::Incomplete::selectValues(property->id());

		for (Entity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
			if (parents.at(i) != entity)
			{
				sqlQuery = QString(query).
						   arg(QString::number(parents.at(i)->id())).
						   arg(Database::idsToString(ids)).toUtf8();

				if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
				{
					for (int res = sqlite3_step(statement); res == SQLITE_ROW && !ids.isEmpty(); res = sqlite3_step(statement))
						ids.remove(sqlite3_column_int(statement, 0));

					sqlite3_finalize(statement);
				}
				else
				{
					setLastError(sqlQuery);
					return false;
				}

				if (ids.isEmpty())
					break;
			}
	}

	return true;
}

bool Storage::removeSelfOverlappingIds(Entity *entity, const IdsList &entityIds, Entity *property, IdsSet &propertyIds) const
{
	if (!propertyIds.isEmpty())
	{
		sqlite3_stmt *statement;
		QByteArray sqlQuery = PropertiesTable::selectValuesExcept(entity->id(), entityIds, property->id(), propertyIds);

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW && !propertyIds.isEmpty(); res = sqlite3_step(statement))
				propertyIds.remove(sqlite3_column_int(statement, 0));

			sqlite3_finalize(statement);
		}
		else
		{
			setLastError(sqlQuery);
			return false;
		}
	}

	return true;
}

bool Storage::cleanupParentsValues(Entity *entity) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	const Entity::Parents &parents = entity->parents();
	QString query = PropertiesTable::Incomplete::dropProperty(entity->id());

	for (Entity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
	{
		sqlQuery = QString(query).arg(QString::number(parents.at(i)->id())).toUtf8();

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) != SQLITE_OK)
		{
			setLastError(sqlQuery, errorMsg);
			res = false;
			break;
		}
	}

	sqlite3_free(errorMsg);
	return res;
}

bool Storage::cleanupParentsValues(Entity *entity, const IdsList &ids) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	const Entity::Parents &parents = entity->parents();
	QString query = PropertiesTable::Incomplete::removeValues(entity->id(), ids);

	for (Entity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
	{
		sqlQuery = QString(query).arg(QString::number(parents.at(i)->id())).toUtf8();

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) != SQLITE_OK)
		{
			setLastError(sqlQuery, errorMsg);
			res = false;
			break;
		}
	}

	sqlite3_free(errorMsg);
	return res;
}

bool Storage::cleanupPropertyValues(Entity *entity) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	QString query = PropertiesTable::Incomplete::dropProperty2(entity->id());

	for (Entity::size_type i = 0, size = entity->size(); i < size; ++i)
		if (cleanupPropertyValues(entity, entity->at(i).entity))
		{
			sqlQuery = QString(query).append(QString::number(entity->at(i).entity->id())).toUtf8();

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) != SQLITE_OK)
			{
				setLastError(sqlQuery, errorMsg);
				res = false;
				break;
			}
		}
		else
		{
			res = false;
			break;
		}

	sqlite3_free(errorMsg);
	return res;
}

bool Storage::cleanupPropertyValues(Entity *entity, const IdsList &ids) const
{
	IdsSet propertyIds;
	QByteArray sqlQuery;
	sqlite3_stmt *statement;
	QString query = PropertiesTable::Incomplete::selectValues(entity->id(), ids);

	for (Entity::size_type i = 0, size = entity->size(); i < size; ++i)
	{
		propertyIds.clear();
		sqlQuery = QString(query).arg(QString::number(entity->at(i).entity->id())).toUtf8();

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
				propertyIds.insert(sqlite3_column_int(statement, 0));

			sqlite3_finalize(statement);

			if (!removeSelfOverlappingIds(entity, ids, entity->at(i).entity, propertyIds) ||
				!removeOverlappingIds(entity, entity->at(i).entity, propertyIds) ||
				!removeValue(entity->at(i).entity, propertyIds.toList()))
				return false;
		}
		else
		{
			setLastError(sqlQuery);
			return false;
		}
	}

	return true;
}

bool Storage::cleanupPropertyValues(Entity *entity, Entity *property) const
{
	IdsSet ids;
	sqlite3_stmt *statement;
	QByteArray sqlQuery = EntitiesTable::selectValues(entity->id(), property->id());

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			ids.insert(sqlite3_column_int(statement, 0));

		sqlite3_finalize(statement);

		if (removeOverlappingIds(entity, property, ids))
			return removeValue(property, ids.toList());
	}
	else
		setLastError(sqlQuery);

	return false;
}

void Storage::loadEntities()
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = EntitiesTable::select();

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		Entity *entity;

		for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
		{
			entity = new Entity(
					static_cast<Entity::Type>(sqlite3_column_int(statement, 1)),
					sqlite3_column_int(statement, 0),
					QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)),
					QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)),
					QByteArray((const char *)sqlite3_column_blob(statement, 4), sqlite3_column_bytes(statement, 4)),
					QByteArray((const char *)sqlite3_column_blob(statement, 5), sqlite3_column_bytes(statement, 5)));
			m_entities.add(entity, entity->name());
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
		m_valid = false;
	}
}

void Storage::loadProperties()
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = PropertiesTable::select();

	if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		Entity *entity1;
		Entity *entity2;

		for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
		{
			entity1 = m_entities.at(m_entities.indexOf(sqlite3_column_int(statement, 1))).entity;
			entity1->add(entity2 = m_entities.at(m_entities.indexOf(sqlite3_column_int(statement, 2))).entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)));
			entity2->addParent(entity1);
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
		m_valid = false;
	}
}

void Storage::loadEntities(sqlite3_stmt *statement, Entity *parent)
{
	Entity::Type type;
	Entity::id_type id;
	Entity::size_type index;

	for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
		switch (type = static_cast<Entity::Type>(sqlite3_column_int(statement, 1)))
		{
			case Database::Int:
			case Database::String:
			case Database::Date:
			case Database::Time:
			case Database::DateTime:
			case Database::Memo:
			case Database::Rating:
			case Database::Path:
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != Entity::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index).entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index).entity->addParent(parent);
					}
				}
				else
				{
					const void *data;
					QByteArray listGeometry;
					QByteArray editorGeometry;

					if (data = sqlite3_column_blob(statement, 4))
						editorGeometry = QByteArray(static_cast<const char *>(data), sqlite3_column_bytes(statement, 4));

					if (data = sqlite3_column_blob(statement, 5))
						listGeometry = QByteArray(static_cast<const char *>(data), sqlite3_column_bytes(statement, 5));

					Entity *entity = new Entity(type, id,
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)),
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)),
							editorGeometry,
							listGeometry);

					m_entities.add(entity, entity->name());

					if (&m_entities != parent)
					{
						parent->add(entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						entity->addParent(parent);
					}
				}
				break;
			}
			case Database::Composite:
			{
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != Entity::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index).entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index).entity->addParent(parent);
					}
				}
				else
				{
					const void *data;
					QByteArray listGeometry;
					QByteArray editorGeometry;
					sqlite3_stmt *statement2;

					if (data = sqlite3_column_blob(statement, 4))
						editorGeometry = QByteArray(static_cast<const char *>(data), sqlite3_column_bytes(statement, 4));

					if (data = sqlite3_column_blob(statement, 5))
						listGeometry = QByteArray(static_cast<const char *>(data), sqlite3_column_bytes(statement, 5));

					Entity *entity = new Entity(Database::Composite, id,
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)),
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)),
							editorGeometry,
							listGeometry);

					QByteArray query = PropertiesTable::select(entity->id());

					m_entities.add(entity, entity->name());

					if (&m_entities != parent)
					{
						parent->add(entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)));
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

void Storage::copyEntities(sqlite3 *oldDb, QByteArray &sqlQuery)
{
	sqlite3_stmt *statement;
	sqlQuery = EntitiesTable::select();

	if (sqlite3_prepare_v2(oldDb, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		sqlite3_stmt *insert;
		sqlQuery = EntitiesTable::insert();

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &insert, NULL) == SQLITE_OK)
		{
			Entity::id_type id;
			Entity::Type type;
			Entity *entity;
			QByteArray text1;
			QByteArray text2;
			QByteArray buffer1;
			QByteArray buffer2;

			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			{
				if (sqlite3_bind_int(insert, 1, id = sqlite3_column_int(statement, 0)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_bind_int(insert, 2, type = static_cast<Entity::Type>(sqlite3_column_int(statement, 1))) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				text1 = (const char *)sqlite3_column_text(statement, 2);

				if (sqlite3_bind_text(insert, 3, text1.data(), text1.size(), SQLITE_STATIC) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				text2 = (const char *)sqlite3_column_text(statement, 3);

				if (sqlite3_bind_text(insert, 4, text2.data(), text2.size(), SQLITE_STATIC) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				buffer1 = QByteArray((const char *)sqlite3_column_blob(statement, 4), sqlite3_column_bytes(statement, 4));

				if (sqlite3_bind_blob(insert, 5, buffer1.data(), buffer1.size(), SQLITE_STATIC) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				buffer2 = QByteArray((const char *)sqlite3_column_blob(statement, 5), sqlite3_column_bytes(statement, 5));

				if (sqlite3_bind_blob(insert, 6, buffer1.data(), buffer1.size(), SQLITE_STATIC) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_step(insert) == SQLITE_DONE)
					if (sqlite3_reset(insert) == SQLITE_OK)
					{
						entity = new Entity(type, id, QString::fromUtf8(text1), QString::fromUtf8(text2), buffer1, buffer2);
						m_entities.add(entity, entity->name());
					}
					else
					{
						setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
						m_valid = false;
						break;
					}
				else
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}
			}

			if (sqlite3_finalize(insert) != SQLITE_OK)
			{
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			m_valid = false;
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

void Storage::copyProperties(sqlite3 *oldDb, QByteArray &sqlQuery)
{
	sqlite3_stmt *statement;
	sqlQuery = PropertiesTable::select();

	if (sqlite3_prepare_v2(oldDb, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		sqlite3_stmt *insert;
		sqlQuery = PropertiesTable::insert();

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &insert, NULL) == SQLITE_OK)
		{
			Entity::id_type id;
			Entity::id_type entityId;
			Entity::id_type propertyId;
			Entity *entity1;
			Entity *entity2;
			QByteArray text;

			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			{
				if (sqlite3_bind_int(insert, 1, id = sqlite3_column_int(statement, 0)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_bind_int(insert, 2, entityId = sqlite3_column_int(statement, 1)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_bind_int(insert, 3, propertyId = sqlite3_column_int(statement, 2)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				text = (const char *)sqlite3_column_text(statement, 3);

				if (sqlite3_bind_text(insert, 4, text.data(), text.size(), SQLITE_STATIC) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_step(insert) == SQLITE_DONE)
					if (sqlite3_reset(insert) == SQLITE_OK)
					{
						entity1 = m_entities.at(m_entities.indexOf(entityId)).entity;
						entity1->add(entity2 = m_entities.at(m_entities.indexOf(propertyId)).entity, QString::fromUtf8(text));
						entity2->addParent(entity1);
					}
					else
					{
						setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
						m_valid = false;
						break;
					}
				else
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}
			}

			if (sqlite3_finalize(insert) != SQLITE_OK)
			{
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
			m_valid = false;
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

void Storage::copySingleEntityValues(sqlite3 *oldDb, Entity *entity, QByteArray &sqlQuery)
{
	sqlite3_stmt *statement;
	sqlQuery = EntitiesTable::select(entity->id());

	if (sqlite3_prepare_v2(oldDb, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		QByteArray buffer;
		sqlite3_stmt *insert;
		sqlQuery = EntitiesTable::insertIntoSingle(entity->id());

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &insert, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			{
				if (sqlite3_bind_int(insert, 1, sqlite3_column_int(statement, 0)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (!Database::bind(entity->type(), statement, 1, insert, 2, buffer))
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_step(insert) == SQLITE_DONE)
				{
					if (sqlite3_reset(insert) != SQLITE_OK)
					{
						setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
						m_valid = false;
						break;
					}
				}
				else
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}
			}

			if (sqlite3_finalize(insert) != SQLITE_OK)
			{
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

void Storage::copyCompositeEntityValues(sqlite3 *oldDb, Entity *entity, QByteArray &sqlQuery)
{
	sqlite3_stmt *statement;
	sqlQuery = EntitiesTable::select(entity->id());

	if (sqlite3_prepare_v2(oldDb, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		sqlite3_stmt *insert;
		sqlQuery = EntitiesTable::insertIntoComposite(entity->id());

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &insert, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			{
				if (sqlite3_bind_int(insert, 1, sqlite3_column_int(statement, 0)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_step(insert) == SQLITE_DONE)
				{
					if (sqlite3_reset(insert) != SQLITE_OK)
					{
						setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
						m_valid = false;
						break;
					}
				}
				else
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}
			}

			if (sqlite3_finalize(insert) != SQLITE_OK)
			{
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

void Storage::copyPropertyValues(sqlite3 *oldDb, Entity *entity, Entity *property, QByteArray &sqlQuery)
{
	sqlite3_stmt *statement;
	sqlQuery = PropertiesTable::select(entity->id(), property->id());

	if (sqlite3_prepare_v2(oldDb, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
	{
		sqlite3_stmt *insert;
		sqlQuery = PropertiesTable::insert(entity->id(), property->id());

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &insert, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
			{
				if (sqlite3_bind_int(insert, 1, sqlite3_column_int(statement, 0)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_bind_int(insert, 2, sqlite3_column_int(statement, 1)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_bind_int(insert, 3, sqlite3_column_int(statement, 2)) != SQLITE_OK)
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}

				if (sqlite3_step(insert) == SQLITE_DONE)
				{
					if (sqlite3_reset(insert) != SQLITE_OK)
					{
						setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
						m_valid = false;
						break;
					}
				}
				else
				{
					setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
					m_valid = false;
					break;
				}
			}

			if (sqlite3_finalize(insert) != SQLITE_OK)
			{
				setLastError(QString::fromUtf8(sqlite3_errmsg(m_db)));
				m_valid = false;
			}
		}
		else
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}

		if (sqlite3_finalize(statement) != SQLITE_OK)
		{
			setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
			m_valid = false;
		}
	}
	else
	{
		setLastError(QString::fromUtf8(sqlite3_errmsg(oldDb)));
		m_valid = false;
	}
}

void Storage::performUndo()
{
	for (UndoStack::size_type i = m_undo.size() - 1; i >= 0; --i)
	{
		UndoList &list = m_undo[i];

		for (UndoList::size_type i = list.size() - 1; i >= 0; --i)
		{
			list.at(i)->undo(m_entities);
			delete list.at(i);
		}
	}

	m_undo.clear();
}

void Storage::performUndo(const QByteArray &name)
{
	UndoList list;

	for (UndoStack::size_type i = m_undo.size() - 1, index = m_undo.indexOf(name); i >= index; --i)
	{
		list = m_undo.take(m_undo.size() - 1);

		for (UndoList::size_type i = list.size() - 1; i >= 0; --i)
		{
			list.at(i)->undo(m_entities);
			delete list.at(i);
		}
	}
}

void Storage::clearUndoStack()
{
	for (UndoStack::size_type i = 0, size = m_undo.size(); i < size; ++i)
		qDeleteAll(m_undo.at(i));

	m_undo.clear();
}

void Storage::clearUndoStack(const QByteArray &name)
{
	UndoStack::size_type index;
	UndoList &list = m_undo[(index = m_undo.indexOf(name)) - 1];

	for (UndoStack::size_type i = index, size = m_undo.size(); i < size; ++i)
		list += m_undo.take(index);
}

QString Storage::failedToLoadId(const QString &tableName) const
{
	return tr("Failed to load id for table \"%1\"").arg(tableName);
}

QString Storage::failedToBind(const QByteArray &sqlQuery) const
{
	return tr("Failed to bind sql parameters: \"%1\"").arg(QString::fromLatin1(sqlQuery));
}

QString Storage::failedToReset(const QByteArray &sqlQuery) const
{
	return tr("Failed to reset: \"%1\"").arg(QString::fromLatin1(sqlQuery));
}

void Storage::setLastError(const QByteArray &sqlQuery, sqlite3 *db) const
{
	m_lastError = QString::fromLatin1("Failed to execute \"%1\" query: ").
			arg(QString::fromLatin1(sqlQuery)).
			append(QString::fromUtf8(sqlite3_errmsg(db)));
}

void Storage::setLastError(const char *sqlQuery) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query").arg(QString::fromLatin1(sqlQuery));
}

void Storage::setLastError(const char *sqlQuery, const char *errorMsg) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query (%2)").arg(QString::fromLatin1(sqlQuery)).arg(QString::fromUtf8(errorMsg));
}

void Storage::setLastError(const QByteArray &sqlQuery) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query").arg(QString::fromLatin1(sqlQuery));
}

void Storage::setLastError(const QByteArray &sqlQuery, const char *errorMsg) const
{
	m_lastError = QString::fromLatin1("Failed to prepare \"%1\" query (%2)").arg(QString::fromLatin1(sqlQuery)).arg(QString::fromUtf8(errorMsg));
}

void Storage::setLastError(const QString &error) const
{
	m_lastError = error;
}

IDM_PLUGIN_NS_END
