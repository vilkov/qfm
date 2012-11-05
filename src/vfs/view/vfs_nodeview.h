/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef VFS_NODEVIEW_H_
#define VFS_NODEVIEW_H_

#include <QtGui/QWidget>
#include <tools/events/qt/imp/mouseeventhandler.h>
#include <tools/events/qt/imp/keyboardeventhandler.h>
#include <tools/events/qt/imp/contextmenueventhandler.h>
#include <tools/events/qt/imp/mouseeventsource.h>
#include <tools/events/qt/imp/keyboardeventsource.h>
#include <tools/events/qt/imp/contextmenueventsource.h>
#include <tools/events/qt/imp/focuseventhandler.h>
#include <tools/events/qt/imp/eventhandlerdefault.h>
#include "../interfaces/vfs_inodeview.h"
#include "../../history/navigationhistory.h"


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
