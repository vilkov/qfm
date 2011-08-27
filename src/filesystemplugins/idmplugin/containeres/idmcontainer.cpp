#include "idmcontainer.h"
#include "menu/idmmenuentities.h"
#include "../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmContainer::IdmContainer(const Info &storage) :
	m_menu(tr("Menu"), tr("Main menu")),
	m_storage(storage)
{
	if (m_storage.isValid())
	{
		PScopedPointer<IdmMenuEntities> entities(new IdmMenuEntities(tr("List"), tr("List of the all entities"), &m_menu));

		m_menu.add(Create, tr("Create"), tr("Create an entity"));
		m_menu.add(Query, tr("Query"), tr("Create a query"));

		for (IdmStorage::size_type i = 0, size = m_storage.size(); i < size; ++i)
			entities->add(m_storage.at(i));

		m_menu.add(entities.take());
	}

	m_entityTypes[IdmEntity::Int]       = IdmEntityTypeDescription(tr("Int"),       tr("Integer type"));
	m_entityTypes[IdmEntity::String]    = IdmEntityTypeDescription(tr("String"),    tr("String type (max length is 1024 characters)"));
	m_entityTypes[IdmEntity::Date]      = IdmEntityTypeDescription(tr("Date"),      tr("Date type"));
	m_entityTypes[IdmEntity::Time]      = IdmEntityTypeDescription(tr("Time"),      tr("Time type"));
	m_entityTypes[IdmEntity::DateTime]  = IdmEntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
	m_entityTypes[IdmEntity::Memo]      = IdmEntityTypeDescription(tr("Memo"),      tr("Memo type"));
	m_entityTypes[IdmEntity::Composite] = IdmEntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
	m_entityTypes[IdmEntity::Rating]    = IdmEntityTypeDescription(tr("Rating"),    tr("Rating type (shown as stars)"));
	m_entityTypes[IdmEntity::Path]      = IdmEntityTypeDescription(tr("Path"),      tr("Path to file or folder"));
}

IDM_PLUGIN_NS_END
