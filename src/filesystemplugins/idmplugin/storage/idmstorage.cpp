#include "idmstorage.h"
#include "values/idmvaluereader.h"
#include "structure/idmentitiestable.h"
#include "structure/idmpropertiestable.h"
#include "undo/concrete/idmstorageundoaddentity.h"
#include "undo/concrete/idmstorageundoaddproperty.h"
#include "undo/concrete/idmstorageundoremoveentity.h"
#include "undo/concrete/idmstorageundoremoveproperty.h"
#include "undo/concrete/idmstorageundoupdatevalue.h"
#include "undo/concrete/idmstorageundoaddvalueproperty.h"
#include "undo/concrete/idmstorageundoaddvalue.h"
#include "../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmStorage::IdmStorage(const Info &storage) :
	m_info(storage),
	m_valid(true),
	m_db(0)
{
	if (m_info.exists())
		if (sqlite3_open16(m_info.absoluteFilePath().unicode(), &m_db) == SQLITE_OK)
		{
			sqlite3_stmt *statement;
			QByteArray sqlQuery = EntitiesTable::select();

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
				setLastError(sqlQuery.data());
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
			QByteArray sqlQuery = Database::init();

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &error) != SQLITE_OK)
			{
				m_valid = false;
				setLastError(sqlQuery.data(), error);
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

bool IdmStorage::transaction()
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
		}
	}

	return false;
}

bool IdmStorage::commit()
{
	char *errorMsg = 0;
	static const char *sqlQuery = "commit";

	if (sqlite3_exec(m_db, sqlQuery, NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		clearUndoStack();
		return true;
	}
	else
	{
		setLastError(sqlQuery, errorMsg);
		return false;
	}
}

void IdmStorage::rollback()
{
	char *errorMsg = 0;
	static const char *sqlQuery = "rollback";

	if (sqlite3_exec(m_db, sqlQuery, NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo();
	else
		setLastError(sqlQuery, errorMsg);
}

bool IdmStorage::savepoint(const QByteArray &name)
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
	}

	return false;
}

bool IdmStorage::release(const QByteArray &name)
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
	{
		setLastError(sqlQuery, errorMsg);
		return false;
	}
}

void IdmStorage::rollback(const QByteArray &name)
{
	char *errorMsg = 0;
	QByteArray sqlQuery("rollback to '");
	sqlQuery.append(name).append('\'');

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo(name);
	else
		setLastError(sqlQuery, errorMsg);
}

QueryContext IdmStorage::prepare(const Query &query, QString &error) const
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

IdmEntity *IdmStorage::createEntity(const QString &name, IdmEntity::Type type, const IdmShortFormat &shortFormat)
{
	IdmEntity *res = 0;
	IdmEntity::id_type id = loadId("ENTITY");

	if (id != IdmEntity::InvalidId)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = EntitiesTable::insert(type, id, name, shortFormat.format());

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			m_entities.add(res = new IdmEntity(type, id, name, shortFormat), name);
			m_undo.last().push_back(new IdmStorageUndoAddEntity(res));
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}

	return res;
}

bool IdmStorage::removeEntity(IdmEntity *entity)
{
	char *errorMsg = 0;
	QByteArray sqlQuery = EntitiesTable::remove(entity->id());

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		if (cleanupParentsValues(entity) &&
			(entity->type() != Database::Composite || cleanupPropertyValues(entity)))
		{
			IdmEntity *parent;
			PScopedPointer<IdmStorageUndoRemoveEntity> command(new IdmStorageUndoRemoveEntity(entity));
			const IdmEntity::Parents &parents = entity->parents();

			for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
			{
				parent = parents.at(i);
				command->addParent(parent, parent->at(parent->indexOf(entity)).name);
				parents.at(i)->remove(entity);
			}

			for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
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

bool IdmStorage::addProperty(IdmEntity *entity, IdmEntity *property, const QString &name)
{
	if (entity->id() != property->id() &&
		entity->type() == Database::Composite &&
		entity->indexOf(property->id()) == IdmEntity::InvalidIndex &&
		!isThereCycles(entity, property))
	{
		IdmEntity::id_type id = loadId(PropertiesTable::tableName());

		if (id != IdmEntity::InvalidId)
		{
			char *errorMsg = 0;
			QByteArray sqlQuery = PropertiesTable::insert(id, entity->id(), property->id(), name);

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				entity->add(property, name);
				property->addParent(entity);
				m_undo.last().push_back(new IdmStorageUndoAddProperty(entity, property));

				return true;
			}
			else
				setLastError(sqlQuery, errorMsg);

			sqlite3_free(errorMsg);
		}
	}

	return false;
}

bool IdmStorage::removeProperty(IdmEntity *entity, IdmEntity *property)
{
	if (entity->id() != property->id() &&
		entity->type() == Database::Composite &&
		entity->indexOf(property->id()) != IdmEntity::InvalidIndex)
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
					PScopedPointer<IdmStorageUndoRemoveProperty> command(new IdmStorageUndoRemoveProperty(entity, property, entity->at(entity->indexOf(property)).name));
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

IdmCompositeEntityValue *IdmStorage::addValue(IdmEntity *entity) const
{
	QString str;
	id_type id = loadId(str = QString::fromLatin1("ENTITY_").append(QString::number(entity->id())));

	if (id != IdmEntity::InvalidId)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = EntitiesTable::addValue(entity->id(), id);

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			return IdmValueReader::createValue(entity, id);
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}
	else
		setLastError(failedToLoadId(str));

	return 0;
}

bool IdmStorage::addValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue) const
{
	QString table =
			QString::fromLatin1("ENTITY_").append(QString::number(entityValue->entity()->id())).
			QString::fromLatin1("_PROPERTY_").append(QString::number(propertyValue->entity()->id()));
	id_type id = loadId(table);

	if (id != IdmEntity::InvalidId)
	{
		char *errorMsg = 0;
		sqlite3_stmt *statement;
		QByteArray sqlQuery = PropertiesTable::addValue(table, id, entityValue->id(), propertyValue->id());

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			m_undo.last().push_back(new IdmStorageUndoAddValueProperty(entityValue, propertyValue));
			IdmValueReader::addValue(entityValue, propertyValue);

			return true;
		}
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}
	else
		setLastError(failedToLoadId(table));

	return false;
}

IdmEntityValue *IdmStorage::addValue(IdmEntity *entity, const QVariant &value) const
{
	QString str;
	id_type id = loadId(str = QString::fromLatin1("ENTITY_").append(QString::number(entity->id())));

	if (id != InvalidId)
	{
		char *errorMsg = 0;
		QByteArray sqlQuery = EntitiesTable::addValue(entity->id(), entity->type(), id, value);

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			return IdmValueReader::createValue(entity, id, value);
		else
			setLastError(sqlQuery, errorMsg);

		sqlite3_free(errorMsg);
	}
	else
		setLastError(failedToLoadId(str));

	return 0;
}

bool IdmStorage::updateValue(IdmEntityValue *value, const QVariant &newValue) const
{
	char *errorMsg = 0;
	QByteArray sqlQuery = EntitiesTable::updateValue(value->entity()->id(), value->entity()->type(), value->id(), newValue);

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		m_undo.last().push_back(new IdmStorageUndoUpdateValue(value));
		IdmValueReader::updateValue(value, newValue);

		return true;
	}
	else
		setLastError(sqlQuery, errorMsg);

	sqlite3_free(errorMsg);

	return false;
}

bool IdmStorage::removeValue(IdmEntity *entity, const IdsList &ids) const
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

IdmStorage::id_type IdmStorage::loadId(const QString &tableName) const
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = Database::loadId(tableName);
	id_type res = IdmEntity::InvalidId;

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

bool IdmStorage::isThereCycles(IdmEntity *entity, IdmEntity *property) const
{
	const IdmEntity::Parents &parents = entity->parents();

	for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
		if (parents.at(i)->id() == property->id())
			return true;
		else
			if (isThereCycles(parents.at(i), property))
				return true;

	return false;
}

bool IdmStorage::removeEntityValues(IdmEntity *entity, const IdsList &ids) const
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

bool IdmStorage::removeOverlappingIds(IdmEntity *entity, IdmEntity *property, IdsSet &ids) const
{
	if (!ids.isEmpty())
	{
		QByteArray sqlQuery;
		sqlite3_stmt *statement;
		const IdmEntity::Parents &parents = property->parents();
		QString query = PropertiesTable::Incomplete::selectValues(property->id());

		for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
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

bool IdmStorage::cleanupParentsValues(IdmEntity *entity) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	const IdmEntity::Parents &parents = entity->parents();
	QString query = PropertiesTable::Incomplete::dropProperty(entity->id());

	for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
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

bool IdmStorage::cleanupParentsValues(IdmEntity *entity, const IdsList &ids) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	const IdmEntity::Parents &parents = entity->parents();
	QString query = PropertiesTable::Incomplete::removeValues(entity->id(), ids);

	for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
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

bool IdmStorage::cleanupPropertyValues(IdmEntity *entity) const
{
	bool res = true;
	char *errorMsg = 0;
	QByteArray sqlQuery;
	sqlite3_stmt *statement;
	QString query = PropertiesTable::Incomplete::dropProperty2(entity->id());

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
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

bool IdmStorage::cleanupPropertyValues(IdmEntity *entity, const IdsList &ids) const
{
	IdsSet propertyIds;
	QByteArray sqlQuery;
	sqlite3_stmt *statement;
	QString query = PropertiesTable::Incomplete::selectValues(entity->id(), ids);

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
	{
		propertyIds.clear();
		sqlQuery = QString(query).arg(QString::number(entity->at(i).entity->id())).toUtf8();

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
				propertyIds.insert(sqlite3_column_int(statement, 0));

			sqlite3_finalize(statement);

			if (!removeOverlappingIds(entity, entity->at(i).entity, propertyIds) ||
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

bool IdmStorage::cleanupPropertyValues(IdmEntity *entity, IdmEntity *property) const
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

void IdmStorage::loadEntities(sqlite3_stmt *statement, IdmEntity *parent)
{
	IdmEntity::Type type;
	IdmEntity::id_type id;
	IdmEntity::size_type index;

	for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
		switch (type = static_cast<IdmEntity::Type>(sqlite3_column_int(statement, 1)))
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
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntity::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index).entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index).entity->addParent(parent);
					}
				}
				else
				{
					IdmEntity *entity = new IdmEntity(type, id,
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)),
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)));

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
				if ((index = m_entities.indexOf(id = sqlite3_column_int(statement, 0))) != IdmEntity::InvalidIndex)
				{
					if (&m_entities != parent)
					{
						parent->add(m_entities.at(index).entity, QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index).entity->addParent(parent);
					}
				}
				else
				{
					sqlite3_stmt *statement2;
					IdmEntity *entity = new IdmEntity(Database::Composite, id,
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 2)),
							QString::fromUtf8((const char *)sqlite3_column_text(statement, 3)));
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

void IdmStorage::performUndo()
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

void IdmStorage::performUndo(const QByteArray &name)
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

void IdmStorage::clearUndoStack()
{
	for (UndoStack::size_type i = 0, size = m_undo.size(); i < size; ++i)
		qDeleteAll(m_undo.at(i));

	m_undo.clear();
}

void IdmStorage::clearUndoStack(const QByteArray &name)
{
	UndoStack::size_type index;
	UndoList &list = m_undo[(index = m_undo.indexOf(name)) - 1];

	for (UndoStack::size_type i = index, size = m_undo.size(); i < size; ++i)
		list.append(m_undo.take(index));
}

QString IdmStorage::failedToLoadId(const QString &tableName) const
{
	return tr("Failed to load id for table \"%1\"").arg(tableName);
}

QString IdmStorage::failedToBind(const QByteArray &sqlQuery) const
{
	return tr("Failed to bind sql parameters: \"%1\"").arg(QString::fromLatin1(sqlQuery));
}

QString IdmStorage::failedToReset(const QByteArray &sqlQuery) const
{
	return tr("Failed to reset: \"%1\"").arg(QString::fromLatin1(sqlQuery));
}

void IdmStorage::setLastError(const QByteArray &sqlQuery, sqlite3 *db) const
{
	m_lastError = QString::fromLatin1("Failed to execute \"%1\" query: ").
			arg(QString::fromLatin1(sqlQuery)).
			append(QString::fromUtf8(sqlite3_errmsg(db)));
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

IDM_PLUGIN_NS_END
