#ifndef IDMCONTAINER_H_
#define IDMCONTAINER_H_

#include <QtCore/QList>
#include <QtCore/QCoreApplication>
#include <QtGui/QAction>
#include "idmentitytypes.h"
#include "../storage/idmstorage.h"


IDM_PLUGIN_NS_BEGIN

class IdmContainer
{
	Q_DECLARE_TR_FUNCTIONS(IdmContainer)

public:
	typedef IdmStorage::id_type   id_type;
	typedef IdmStorage::size_type size_type;
	enum { InvalidId = IdmStorage::InvalidId };
	enum { InvalidIndex = IdmStorage::InvalidIndex };

	enum MenuId
	{
		Create,
		Query,
		List
	};

public:
	IdmContainer(const Info &storage);
	~IdmContainer();

	const QList<QAction*> &menuActions() const { return m_menuActions; }
	const IdmEntityTypes &entityTypes() const { return m_entityTypes; }

	/* IdmStorage */
	bool isValid() const { return m_storage.isValid(); }
	const QString &lastError() const { return m_storage.lastError(); }

	IdmEntity *at(size_type index) const { return m_storage.at(index); }
	size_type size() const { return m_storage.size(); }
	size_type indexOf(IdmEntity *item) const { return m_storage.indexOf(item); }
	size_type indexOf(id_type id) const { return m_storage.indexOf(id); }

	bool transaction() { return m_storage.transaction(); }
	bool commit() { return m_storage.commit(); }
	void rollback() { m_storage.rollback(); }

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type) { return m_storage.createEntity(name, type); }
	bool removeEntity(IdmEntity *entity) { return m_storage.removeEntity(entity); }

	bool addProperty(IdmEntity *entity, IdmEntity *property) { return m_storage.addProperty(entity, property); }
	bool removeProperty(IdmEntity *entity, IdmEntity *property) { return m_storage.removeProperty(entity, property); }

private:
	IdmStorage m_storage;
	IdmEntityTypes m_entityTypes;
	QList<QAction*> m_menuActions;
};

IDM_PLUGIN_NS_END

#endif /* IDMCONTAINER_H_ */
