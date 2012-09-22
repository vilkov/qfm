#ifndef VFS_NODEVIEW_H_
#define VFS_NODEVIEW_H_

#include <QtGui/QWidget>
#include "../interfaces/vfs_inodeview.h"
#include "../../history/navigationhistory.h"
#include "../../tools/events/qt/imp/mouseeventhandler.h"
#include "../../tools/events/qt/imp/keyboardeventhandler.h"
#include "../../tools/events/qt/imp/contextmenueventhandler.h"
#include "../../tools/events/qt/imp/mouseeventsource.h"
#include "../../tools/events/qt/imp/keyboardeventsource.h"
#include "../../tools/events/qt/imp/contextmenueventsource.h"
#include "../../tools/events/qt/imp/focuseventhandler.h"
#include "../../tools/events/qt/imp/eventhandlerdefault.h"


VFS_NS_BEGIN
using namespace ::Tools::Events;


class NodeView : public QWidget, public INodeView
{
public:
	NodeView(QWidget *parent = 0);

	/* INodeView */
	virtual void save(::History::Entry *entry);

protected:
	::Tools::Events::EventHandler *eventHandler() { return &m_eventHandler; }
	void initializeHistory(::History::Entry *entry) { m_navigation.init(entry); }

protected:
	void goUp();
	void goBackward();
	void goForward();
	void refresh();
	void activated();
	void contextMenu();
	void cancel();

	virtual bool handleShortcut(QEvent *event);

private:
	typedef MouseDoubleClickEventHandler<
				ContextMenuEventHandler<
					KeyboardEventHandler<
						EventHandlerDefault<NodeView>,
						::Tools::Templates::bool_value<true>
					>
				>
			> EventHandler;

private:
	EventHandler m_eventHandler;
    ::History::Navigation m_navigation;
};

VFS_NS_END

#endif /* VFS_NODEVIEW_H_ */
