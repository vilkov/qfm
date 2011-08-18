#ifndef IDMCONTAINER_H_
#define IDMCONTAINER_H_

#include <QtCore/QCoreApplication>
#include "idmentitytypes.h"
#include "menu/idmmenu.h"
#include "../storage/idmstorage.h"


FILE_SYSTEM_NS_BEGIN

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
		Remove
	};

public:
	IdmContainer(const Info &storage);

	const IdmEntityTypes &entityTypes() const { return m_entityTypes; }
	const IdmMenu *menu() const { return &m_menu; }
	IdmMenu *menu() { return &m_menu; }

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
	void removeEntity(IdmEntity *entity) { m_storage.removeEntity(entity); }

	void addProperty(IdmEntity *entity, IdmEntity *property) { m_storage.addProperty(entity, property); }
	void removeProperty(IdmEntity *entity, IdmEntity *property) { m_storage.removeProperty(entity, property); }

private:
	IdmMenu m_menu;
	IdmStorage m_storage;
	IdmEntityTypes m_entityTypes;
};

FILE_SYSTEM_NS_END

#endif /* IDMCONTAINER_H_ */
