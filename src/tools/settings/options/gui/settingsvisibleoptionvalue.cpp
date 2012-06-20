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

QLayout *VisibleOptionValue::createEditor()
{
	PScopedPointer<QHBoxLayout> res(new QHBoxLayout());

	res->addWidget(new QLabel(m_label));
	res->addWidget(m_editor = new QLineEdit(value()));

	return res.take();
}

bool VisibleOptionValue::accept()
{
	return !m_editor->text().isEmpty();
}

void VisibleOptionValue::reject()
{}

SETTINGS_NS_END
