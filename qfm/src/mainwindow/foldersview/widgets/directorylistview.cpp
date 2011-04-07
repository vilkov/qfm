#include "directorylistview.h"


DirectoryListView::DirectoryListView(EventHandler *eventHandler, QWidget *parent) :
	QTreeView(parent),
	m_eventHandler(eventHandler)
{
	Q_ASSERT(m_eventHandler != 0);
}

void DirectoryListView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (!m_eventHandler->mouseDoubleClickEvent(event))
		QTreeView::mouseDoubleClickEvent(event);
}

void DirectoryListView::keyPressEvent(QKeyEvent *event)
{
	if (!m_eventHandler->keyboardEvent(event))
		QTreeView::keyPressEvent(event);
}
