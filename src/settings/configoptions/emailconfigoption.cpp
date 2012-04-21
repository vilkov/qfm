#include "emailconfigoption.h"


EmailConfigOption::EmailConfigOption(qint32 id, const QString &title, ConfigOption *parent) :
	ConfigOptionValueDefault(id, title, QString::fromLatin1("your.email@domain.com"), parent),
	m_editor(0),
	m_listener(0)
{}

bool EmailConfigOption::isEnabled() const
{
	return m_editor->isEnabled();
}

void EmailConfigOption::setEnabled(bool value)
{
	m_editor->setEnabled(value);
	m_button->setEnabled(value);
}

QWidget *EmailConfigOption::createEditor(QWidget *parent)
{
	QScopedPointer<QWidget> widget(new QWidget(parent));
	QHBoxLayout *layout = new QHBoxLayout(widget.data());

	layout->setMargin(1);
	layout->setSpacing(1);
	layout->addWidget(m_editor = new QLineEdit());
	layout->addWidget(m_button = new QPushButton(tr("Test")));
	m_listener->connect(m_button, SIGNAL(clicked()), m_listenerSlot);

	return widget.take();
}

QVariant EmailConfigOption::editorValue() const
{
	return m_editor->text();
}

void EmailConfigOption::setEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void EmailConfigOption::setLoadedEditorValue(const QVariant &value)
{
	m_editor->setText(value.toString());
}

void EmailConfigOption::setListener(QObject *listener, const char *listenerSlot)
{
	m_listener = listener; m_listenerSlot = listenerSlot;
}
