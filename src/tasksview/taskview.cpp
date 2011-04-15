#include "taskview.h"


TaskView::TaskView(QWidget *parent) :
	QWidget(parent),
	m_eventHandler(this),
	m_layout(this),
	m_text(this),
	m_cancel(&m_eventHandler, this)
{
	setLayout(&m_layout);
	m_layout.addWidget(&m_text);
	m_layout.addWidget(&m_cancel);

	m_eventHandler.registerMouseReleaseEventHandler(&TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Enter,  &TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Space,  &TaskView::deleteLater);
}
