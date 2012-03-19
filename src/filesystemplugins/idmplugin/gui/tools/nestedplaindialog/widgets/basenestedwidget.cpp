#include "basenestedwidget.h"
#include "nesteddialog.h"


BaseNestedWidget::BaseNestedWidget(NestedDialog *parent) :
	m_parent(parent)
{}

BaseNestedWidget::~BaseNestedWidget()
{}

void BaseNestedWidget::warning(const QString &text)
{
	m_parent->warning(text);
}

void BaseNestedWidget::warning(const QString &title, const QString &text)
{
	m_parent->warning(title, text);
}

void BaseNestedWidget::critical(const QString &text)
{
	m_parent->critical(text);
}

void BaseNestedWidget::critical(const QString &title, const QString &text)
{
	m_parent->critical(title, text);
}

void BaseNestedWidget::accept()
{
	m_parent->accepted();
}

void BaseNestedWidget::reject()
{
	m_parent->rejected();
}
