#include "settingsradiobutton.h"

#include <QtGui/QRadioButton>


SETTINGS_NS_BEGIN

RadioButton::RadioButton(const QString &title, const QString &id, Option *parent, bool defaultValue) :
	OptionValue(id, parent, defaultValue ? QChar(L'1') : QChar(L'0')),
	m_title(title),
	m_editor(NULL)
{}

QWidget *RadioButton::createEditor()
{
	m_editor = new QRadioButton(m_title);
	static_cast<QRadioButton *>(m_editor)->setChecked(value());
	return m_editor;
}

void RadioButton::restoreDefault()
{
	static_cast<QRadioButton *>(m_editor)->setChecked(defaultValue().at(0) == QChar(L'1'));
}

bool RadioButton::accept()
{
	setValue(static_cast<QRadioButton *>(m_editor)->isChecked());
	return true;
}

void RadioButton::reject()
{}

SETTINGS_NS_END
