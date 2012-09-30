#ifndef IDM_CONTAINER_H_
#define IDM_CONTAINER_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QCoreApplication>
#include <QtGui/QAction>

#include <vfs/interfaces/vfs_inodeview.h>
#include <vfs/interfaces/vfs_ifilecontainer.h>

#include "idm_entitytypes.h"
#include "../storage/idm_storage.h"


IDM_PLUGIN_NS_BEGIN

class IdmContainer
{
	Q_DECLARE_TR_FUNCTIONS(IdmContainer)

public:
	typedef Storage::id_type   id_type;
	typedef Storage::size_type size_type;
	typedef Storage::IdsList   IdsList;
	typedef Storage::IdsMap    IdsMap;
	enum { InvalidId = Storage::InvalidId };
	enum { InvalidIndex = Storage::InvalidIndex };

	enum MenuId
	{
		Create,
		Find,
		List
	};

public:
	IdmContainer(IFileContainer::Holder &container, bool create);

	const IFileContainer *container() const { return m_data->container.data(); }
	const INodeView::MenuActionList &menuActions() const { return m_data->menuActions; }
	const EntityTypes &entityTypes() const { return m_data->entityTypes; }

	/* IdmStorage */
	bool isValid() const { return m_data->storage.isValid(); }
	const QString &lastError() const { return m_data->storage.lastError(); }

	Entity *at(size_type index) const { return m_data->storage.at(index); }
	size_type size() const { return m_data->storage.size(); }
	size_type indexOf(id_type id) const { return m_data->storage.indexOf(id); }

	bool transaction() { return m_data->storage.transaction(); }
	bool commit() { return m_data->storage.commit(); }
	void rollback() { m_data->storage.rollback(); }

	bool savepoint(const QByteArray &name) { return m_data->storage.savepoint(name); }
	bool release(const QByteArray &name) { return m_data->storage.release(name); }
	void rollback(const QByteArray &name) { return m_data->storage.rollback(name); }

	QueryContext prepare(const Query &query, QString &error) const { return m_data->storage.prepare(query, error); }

	Entity *createEntity(const QString &name, Entity::Type type, const ShortFormat &shortFormat) { return m_data->storage.createEntity(name, type, shortFormat); }
	bool updateEditorGeometry(Entity *entity, const QRect &geometry) { return m_data->storage.updateEditorGeometry(entity, geometry); }
	bool updateListGeometry(Entity *entity, const QRect &geometry) { return m_data->storage.updateListGeometry(entity, geometry); }
	bool removeEntity(Entity *entity) { return m_data->storage.removeEntity(entity); }

	bool addProperty(Entity *entity, Entity *property, const QString &name) { return m_data->storage.addProperty(entity, property, name); }
	bool renameProperty(Entity *entity, Entity *property, const QString &name) { return m_data->storage.renameProperty(entity, property, name); }
	bool removeProperty(Entity *entity, Entity *property) { return m_data->storage.removeProperty(entity, property); }

	EntityValue::Holder addValue(Entity *entity) const { return m_data->storage.addValue(entity); }
	bool addValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const { return m_data->storage.addValue(entityValue, propertyValue); }
	bool addValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues) const { return m_data->storage.addValue(entityValue, propertyValues); }
	EntityValue::Holder addValue(Entity *entity, const QVariant &value) const { return m_data->storage.addValue(entity, value); }
	bool updateValue(const EntityValue::Holder &value, const QVariant &newValue) const { return m_data->storage.updateValue(value, newValue); }
	bool removeValue(Entity *entity, const IdsList &ids) const { return m_data->storage.removeValue(entity, ids); }
	bool removeValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) const { return m_data->storage.removeValue(entityValue, propertyValue); }

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container, bool create);
		~Data();

		Storage storage;
		EntityTypes entityTypes;
		IFileContainer::Holder container;
		INodeView::MenuActionList menuActions;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CONTAINER_H_ */
