#include "idmrootnode.h"
#include "items/idmmenu.h"
#include "items/idmroot.h"
#include "items/idmmessage.h"
#include "items/idmseparator.h"


FILE_SYSTEM_NS_BEGIN

struct MainMenuItems
{
	enum
	{
		Create,
		Remove
	};
};


IdmRootNode::IdmRootNode(const QFileInfo &storage, Node *parent) :
	IdmNode(storage.absolutePath(), parent),
	m_storage(storage)
{
	if (m_storage.isValid())
	{
		IdmMenu *submenu;
		QScopedPointer<IdmMenu> menu(new IdmMenu(tr("Menu"), tr("Main menu")));

		menu->add(MainMenuItems::Create, tr("Create"), tr("Create an entity"));
		menu->add(MainMenuItems::Remove, tr("Remove"), tr("Remove an entity"));
		submenu = menu->add(tr("List"), tr("List of the all entities"));
		submenu->add(0, tr("Some items"), tr("123"));

		m_items.push_back(menu.take());
		m_items.push_back(new IdmRoot(storage.absolutePath()));
	}
	else
	{
		m_items.push_back(new IdmSeparator());
		m_items.push_back(new IdmMessage(m_storage.lastError()));
	}
}

FILE_SYSTEM_NS_END
