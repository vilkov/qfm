#include "settingsvisibleoptionvalue.h"
#include "../../../pointers/pscopedpointer.h"

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>


SETTINGS_NS_BEGIN

VisibleOptionValue::VisibleOptionValue(const QString &label, const QString &id, Option *parent, const QString &defaultValue) :
	OptionValue(id, parent, defaultValue),
	m_label(label)
{}

QWidget *VisibleOptionValue::createEditor()
{
	PScopedPointer<QWidget> page(new QWidget());
	QHBoxLayout *layout(new QHBoxLayout(page.data()));

	layout->setSpacing(1);
	layout->setMargin(1);

	layout->addWidget(new QLabel(m_label));
	layout->addWidget(m_editor = new QLineEdit(value()));

	return page.take();
}

void VisibleOptionValue::restoreDefault()
{
	m_editor->setText(defaultValue());
}

bool VisibleOptionValue::accept()
{
	if (!m_editor->text().isEmpty())
	{
		setValue(m_editor->text());
		return true;
	}

	return false;
}

void VisibleOptionValue::reject()
{}

SETTINGS_NS_END