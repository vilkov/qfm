#include "idmcontainer.h"


FILE_SYSTEM_NS_BEGIN

IdmContainer::IdmContainer(const QFileInfo &storage) :
	m_menu(tr("Menu"), tr("Main menu")),
	m_storage(storage)
{
	if (m_storage.isValid())
	{
		IdmMenu *submenu;

		m_menu.add(Create, tr("Create"), tr("Create an entity"));
		m_menu.add(Remove, tr("Remove"), tr("Remove an entity"));

		submenu = m_menu.add(tr("List"), tr("List of the all entities"));
		submenu->add(0, tr("Some items"), tr("123"));
	}

	m_entityTypes[IdmEntity::Int]       = IdmEntityTypeDescription(tr("Int"),       tr("Integer type"));
	m_entityTypes[IdmEntity::String]    = IdmEntityTypeDescription(tr("String"),    tr("String type (max length is 1024 characters)"));
	m_entityTypes[IdmEntity::Date]      = IdmEntityTypeDescription(tr("Date"),      tr("Date type"));
	m_entityTypes[IdmEntity::Time]      = IdmEntityTypeDescription(tr("Time"),      tr("Time type"));
	m_entityTypes[IdmEntity::DateTime]  = IdmEntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
	m_entityTypes[IdmEntity::Memo]      = IdmEntityTypeDescription(tr("Memo"),      tr("Memo type"));
	m_entityTypes[IdmEntity::Composite] = IdmEntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
	m_entityTypes[IdmEntity::Rating]    = IdmEntityTypeDescription(tr("Rating"),    tr("Rating type (shown as stars)"));
}

FILE_SYSTEM_NS_END
