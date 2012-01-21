#include "../contextmenu.h"


ContextMenu::ContextMenu()
{
	m_menu.addAction(tr("System actions 1 (Create, etc.)"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Copy/Paste"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Open with..."));
	m_menu.addAction(tr("Plugins actions"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Properties"));
}