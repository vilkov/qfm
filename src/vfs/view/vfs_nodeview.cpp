#include "vfs_nodeview.h"
#include "../interfaces/vfs_inode.h"


VFS_NS_BEGIN

NodeView::NodeView(QWidget *parent) :
	QWidget(parent),
	m_eventHandler(this)
{
	m_eventHandler.registerContextMenuEventHandler(&NodeView::contextMenu);
	m_eventHandler.registerMouseDoubleClickEventHandler(&NodeView::activated);
	m_eventHandler.setDefaultHandler(EventHandler::KeyboardEvent, &NodeView::handleShortcut);

	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Backspace, &NodeView::goUp);
	m_eventHandler.registerShortcut(Qt::ALT,        Qt::Key_Left,      &NodeView::goBackward);
	m_eventHandler.registerShortcut(Qt::ALT,        Qt::Key_Right,     &NodeView::goForward);
	m_eventHandler.registerShortcut(Qt::CTRL,       Qt::Key_F5,        &NodeView::refresh);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Return,    &NodeView::activated);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Enter,     &NodeView::activated);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Escape,    &NodeView::cancel);
}

void NodeView::save(::History::Entry *entry)
{
	m_navigation.save(entry);
}

void NodeView::goUp()
{
	if (::History::Entry *entry = node()->viewParent(this))
		m_navigation.save(entry);
}

void NodeView::goBackward()
{
	if (History::Entry *entry = m_navigation.backward())
		node()->viewHistory(this, entry);
}

void NodeView::goForward()
{
	if (History::Entry *entry = m_navigation.forward())
		node()->viewHistory(this, entry);
}

void NodeView::refresh()
{
	node()->refresh();
}

void NodeView::activated()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		if (::History::Entry *entry = node()->viewChild(this, index))
			m_navigation.save(entry);
}

void NodeView::contextMenu()
{
	node()->contextMenu(selectedIndexes(), this);
}

void NodeView::cancel()
{
	node()->cancel(selectedIndexes(), this);
}

bool NodeView::handleShortcut(QEvent *event)
{
	return node()->shortcut(this, static_cast<QKeyEvent *>(event));
}

VFS_NS_END
