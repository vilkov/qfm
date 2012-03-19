#include "basenestedwidget.h"
#include "nesteddialog.h"


BaseNestedWidget::BaseNestedWidget(NestedDialog *parent) :
	m_parent(parent)
{}

BaseNestedWidget::~BaseNestedWidget()
{}

int BaseNestedWidget::open(NestedWidget *widget)
{
	return m_parent->open(widget);
}

void BaseNestedWidget::critical(const QString &text)
{
	return m_parent->critical(text);
}

void BaseNestedWidget::critical(const QString &title, const QString &text)
{
	return m_parent->critical(title, text);
}

void BaseNestedWidget::accept()
{
	m_parent->accepted();
}

void BaseNestedWidget::reject()
{
	m_parent->rejected();
}
