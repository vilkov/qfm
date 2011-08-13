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
	typedef IdmMenu::value_type   value_type;
	typedef value_type::size_type size_type;

	enum MenuId
	{
		Create,
		Remove
	};

public:
	IdmContainer(const Info &storage);

	const IdmMenu *menu() const { return &m_menu; }
	IdmMenu *menu() { return &m_menu; }

	const IdmEntityTypes &entityTypes() const { return m_entityTypes; }
	const IdmItemsList *entitiesList() const { return isValid() ? static_cast<const IdmItemsList *>(m_menu.at(2)) : 0; }

	/* IdmMenu */
	IdmItem *at(size_type index) const { return m_menu.at(index); }
	size_type size() const { return m_menu.size(); }
	size_type indexOf(IdmItem *item) const { return m_menu.indexOf(item); }

	/* IdmStorage */
	bool isValid() const { return m_storage.isValid(); }
	const QString &lastError() const { return m_storage.lastError(); }

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
