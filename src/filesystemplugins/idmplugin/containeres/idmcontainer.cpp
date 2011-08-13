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
}

FILE_SYSTEM_NS_END
