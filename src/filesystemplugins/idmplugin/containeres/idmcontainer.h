#ifndef IDMCONTAINER_H_
#define IDMCONTAINER_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QCoreApplication>
#include <QtGui/QAction>
#include "idmentitytypes.h"
#include "../storage/idmstorage.h"
#include "../../../filesystem/interfaces/filesysteminodeview.h"


IDM_PLUGIN_NS_BEGIN

class IdmContainer
{
	Q_DECLARE_TR_FUNCTIONS(IdmContainer)

public:
	typedef IdmStorage::id_type   id_type;
	typedef IdmStorage::size_type size_type;
	typedef IdmStorage::IdsList   IdsList;
	typedef IdmStorage::IdsMap    IdsMap;
	enum { InvalidId = IdmStorage::InvalidId };
	enum { InvalidIndex = IdmStorage::InvalidIndex };

	enum MenuId
	{
		Create,
		Find,
		List
	};

public:
	IdmContainer(const QString &storage, bool create);

	const INodeView::MenuActionList &menuActions() const { return m_data->menuActions; }
	const IdmEntityTypes &entityTypes() const { return m_data->entityTypes; }

	/* IdmStorage */
	bool isValid() const { return m_data->storage.isValid(); }
	const QString &lastError() const { return m_data->storage.lastError(); }

	IdmEntity *at(size_type index) const { return m_data->storage.at(index); }
	size_type size() const { return m_data->storage.size(); }
	size_type indexOf(id_type id) const { return m_data->storage.indexOf(id); }

	bool transaction() { return m_data->storage.transaction(); }
	bool commit() { return m_data->storage.commit(); }
	void rollback() { m_data->storage.rollback(); }

	bool savepoint(const QByteArray &name) { return m_data->storage.savepoint(name); }
	bool release(const QByteArray &name) { return m_data->storage.release(name); }
	void rollback(const QByteArray &name) { return m_data->storage.rollback(name); }

	QueryContext prepare(const Query &query, QString &error) const { return m_data->storage.prepare(query, error); }

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type, const IdmShortFormat &shortFormat) { return m_data->storage.createEntity(name, type, shortFormat); }
	bool removeEntity(IdmEntity *entity) { return m_data->storage.removeEntity(entity); }

	bool addProperty(IdmEntity *entity, IdmEntity *property, const QString &name) { return m_data->storage.addProperty(entity, property, name); }
	bool renameProperty(IdmEntity *entity, IdmEntity *property, const QString &name) { return m_data->storage.renameProperty(entity, property, name); }
	bool removeProperty(IdmEntity *entity, IdmEntity *property) { return m_data->storage.removeProperty(entity, property); }

	IdmEntityValue::Holder addValue(IdmEntity *entity) const { return m_data->storage.addValue(entity); }
	bool addValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) const { return m_data->storage.addValue(entityValue, propertyValue); }
	bool addValue(const IdmEntityValue::Holder &entityValue, const IdmCompositeEntityValue::List &propertyValues) const { return m_data->storage.addValue(entityValue, propertyValues); }
	IdmEntityValue::Holder addValue(IdmEntity *entity, const QVariant &value) const { return m_data->storage.addValue(entity, value); }
	bool updateValue(const IdmEntityValue::Holder &value, const QVariant &newValue) const { return m_data->storage.updateValue(value, newValue); }
	bool removeValue(IdmEntity *entity, const IdsList &ids) const { return m_data->storage.removeValue(entity, ids); }
	bool removeValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) const { return m_data->storage.removeValue(entityValue, propertyValue); }

private:
	struct Data : public QSharedData
	{
		Data(const QString &storage, bool create) :
			storage(storage, create)
		{}
		~Data()
		{
			qDeleteAll(menuActions);
		}

		IdmStorage storage;
		IdmEntityTypes entityTypes;
		INodeView::MenuActionList menuActions;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

IDM_PLUGIN_NS_END

#endif /* IDMCONTAINER_H_ */
