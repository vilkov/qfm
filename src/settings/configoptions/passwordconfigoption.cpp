#include "passwordconfigoption.h"


PasswordConfigOption::PasswordConfigOption(qint32 id, const QString &title, ConfigOption *parent) :
	ConfigOptionValueDefault(id, title, QVariant(), parent),
	m_editor(0)
{}

bool PasswordConfigOption::isEnabled() const
{
	return m_editor->isEnabled();
}

void PasswordConfigOption::setEnabled(bool value)
{
	m_editor->setEnabled(value);
}

QWidget *PasswordConfigOption::createEditor(QWidget *parent)
{
	m_editor = new QLineEdit(parent);
	static_cast<QLineEdit*>(m_editor)->setEchoMode(QLineEdit::Password);

	return m_editor;
}

QVariant PasswordConfigOption::editorValue() const
{
	return m_editor->text();
}

void PasswordConfigOption::setEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void PasswordConfigOption::setLoadedEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}
