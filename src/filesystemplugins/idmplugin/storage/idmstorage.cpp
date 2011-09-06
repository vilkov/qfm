#include "idmstorage.h"
#include "structure/idmentitiestable.h"
#include "structure/idmpropertiestable.h"
#include "undo/concrete/idmstorageundoaddentity.h"
#include "undo/concrete/idmstorageundoaddproperty.h"
#include "undo/concrete/idmstorageundoremoveentity.h"
#include "undo/concrete/idmstorageundoremoveproperty.h"
#include "../../../tools/pbytearray/pbytearray.h"
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
			const PByteArray sqlQuery("begin transaction");

			if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
			{
				m_undo.add(name, UndoList());
				return true;
			}
			else
				setLastError(sqlQuery.data(), errorMsg);
		}
	}

	return false;
}

bool IdmStorage::commit()
{
	char *errorMsg = 0;
	const PByteArray sqlQuery("commit");

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		clearUndoStack();
		return true;
	}
	else
	{
		setLastError(sqlQuery.data(), errorMsg);
		return false;
	}
}

void IdmStorage::rollback()
{
	char *errorMsg = 0;
	const PByteArray sqlQuery("rollback");

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo();
	else
		setLastError(sqlQuery.data(), errorMsg);
}

bool IdmStorage::savepoint(const QByteArray &name)
{
	if (m_undo.contains(name))
		setLastError(tr("Savepoint \"%1\" already exists!").arg(QString::fromLatin1(name)));
	else
	{
		char *errorMsg = 0;
		QByteArray sqlQuery("savepoint ");
		sqlQuery.append(name);

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
	QByteArray sqlQuery("release ");
	sqlQuery.append(name);

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
	QByteArray sqlQuery("rollback to ");
	sqlQuery.append(name);

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		performUndo(name);
	else
		setLastError(sqlQuery, errorMsg);
}

QueryContext IdmStorage::prepare(const Query &query) const
{
	return QueryContext();
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
	QByteArray sqlQuery = QString::fromLatin1("delete from ENTITY where ID = %1;"
											  "delete from PROPERTIES where ENTITY_PROPERTY_ID = %1;"
											  "drop table ENTITY_%1").
											  arg(QString::number(entity->id())).toUtf8();

	if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		if (cleanupParentsValues(entity) &&
			(entity->type() != Database::Composite || cleanupPropertyValues(entity)))
		{
			PScopedPointer<IdmStorageUndoRemoveEntity> command(new IdmStorageUndoRemoveEntity(entity));
			const IdmEntity::Parents &parents = entity->parents();

			for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
			{
				command->addParent(parents.at(i), parents.at(i)->property(entity).name);
				parents.at(i)->remove(entity);
			}

			for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
				entity->at(i)->removeParent(entity);

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
		IdmEntity::id_type id = loadId("PROPERTIES");

		if (id != IdmEntity::InvalidId)
		{
			char *errorMsg = 0;
			QByteArray sqlQuery = QString::fromLatin1("insert into PROPERTIES (ID, ENTITY_ID, ENTITY_PROPERTY_ID, NAME) values (%1, %2, %3, '%4');"
													  "create table ENTITY_%2_PROPERTY_%3 (ID int primary key, ENTITY_VALUE_ID int, PROPERTY_VALUE_ID int)").
													  arg(QString::number(id)).
													  arg(QString::number(entity->id())).
													  arg(QString::number(property->id())).
													  arg(name).toUtf8();

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
		QByteArray sqlQuery = QString::fromLatin1("delete from PROPERTIES where ENTITY_ID = %1 and ENTITY_PROPERTY_ID = %2").
												  arg(QString::number(entity->id())).
												  arg(QString::number(property->id())).toUtf8();

		if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
		{
			if (cleanupPropertyValues(entity, property))
			{
				sqlQuery = QString::fromLatin1("drop table ENTITY_%1_PROPERTY_%2").
											   arg(QString::number(entity->id())).
											   arg(QString::number(property->id())).toUtf8();

				if (sqlite3_exec(m_db, sqlQuery.data(), NULL, NULL, &errorMsg) == SQLITE_OK)
				{
					PScopedPointer<IdmStorageUndoRemoveProperty> command(new IdmStorageUndoRemoveProperty(entity, property, entity->property(property).name));
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

bool IdmStorage::addValue(IdmEntity *entity, const IdsMap &values) const
{
	return false;
}

bool IdmStorage::addValue(IdmEntity *entity, const QVariant &value) const
{
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

QString IdmStorage::idsToString(const IdsSet &ids) const
{
	QString res;

	for (IdsSet::const_iterator it = ids.constBegin(), end = ids.constEnd(); it != end; ++it)
		res.append(QString::number(*it)).append(QChar(','));

	res.chop(1);
	return res;
}

QString IdmStorage::idsToString(const IdsList &ids) const
{
	QString res;

	for (IdsList::size_type i = 0, size = ids.size(); i < size; ++i)
		res.append(QString::number(ids.at(i))).append(QChar(','));

	res.chop(1);
	return res;
}

IdmEntity::id_type IdmStorage::loadId(const QString &tableName) const
{
	sqlite3_stmt *statement;
	QByteArray sqlQuery = QString::fromLatin1("select ID from %1 order by ID desc").arg(tableName).toUtf8();
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
	QByteArray sqlQuery = QString::fromLatin1("delete from ENTITY_%1 where ID in (%2)").
								   arg(QString::number(entity->id())).
								   arg(idsToString(ids)).toUtf8();

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
		QString query = QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_%1_PROPERTY_").
											append(QString::number(property->id())).
											append(QString::fromLatin1(" where PROPERTY_VALUE_ID in (%2)"));

		for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
			if (parents.at(i) != entity)
			{
				sqlQuery = QString(query).
						   arg(QString::number(parents.at(i)->id())).
						   arg(idsToString(ids)).toUtf8();

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
	QString query = QString::fromLatin1("drop table ENTITY_%1_PROPERTY_").append(QString::number(entity->id()));

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
	QString query = QString::fromLatin1("delete from ENTITY_%1_PROPERTY_").
									    append(QString::number(entity->id())).
									    append(QString::fromLatin1(" where PROPERTY_VALUE_ID in (%1)").arg(idsToString(ids)));

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
	QString query = QString::fromLatin1("drop table ENTITY_%1_PROPERTY_").arg(QString::number(entity->id()));

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
		if (cleanupPropertyValues(entity, entity->at(i)))
		{
			sqlQuery = QString(query).append(QString::number(entity->at(i)->id())).toUtf8();

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
	QString query = QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_").
									    append(QString::number(entity->id())).
									    append(QString::fromLatin1("_PROPERTY_%1")).
									    append(QString::fromLatin1(" where ENTITY_VALUE_ID in (%1)").arg(idsToString(ids)));

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
	{
		propertyIds.clear();
		sqlQuery = QString(query).arg(QString::number(entity->at(i)->id())).toUtf8();

		if (sqlite3_prepare_v2(m_db, sqlQuery.data(), sqlQuery.size(), &statement, NULL) == SQLITE_OK)
		{
			for (int res = sqlite3_step(statement); res == SQLITE_ROW; res = sqlite3_step(statement))
				propertyIds.insert(sqlite3_column_int(statement, 0));

			sqlite3_finalize(statement);

			if (!removeOverlappingIds(entity, entity->at(i), propertyIds) ||
				!removeValue(entity->at(i), propertyIds.toList()))
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
	QByteArray sqlQuery = QString::fromLatin1("select PROPERTY_VALUE_ID from ENTITY_%1_PROPERTY_%2").
											  arg(QString::number(entity->id())).
											  arg(QString::number(property->id())).toUtf8();

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
						parent->add(m_entities.at(index), QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index)->addParent(parent);
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
						parent->add(m_entities.at(index), QString::fromUtf8((const char *)sqlite3_column_text(statement, 4)));
						m_entities.at(index)->addParent(parent);
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
