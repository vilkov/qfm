#include "checkconfigoption.h"


CheckConfigOption::CheckConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0) :
	ConfigOptionValueDefault(id, title, QVariant(), parent),
	m_editor(0),
	m_listener(0)
{}

bool CheckConfigOption::isEnabled() const
{
	return m_editor->isEnabled();
}

void CheckConfigOption::setEnabled(bool value)
{
	m_editor->setEnabled(value);
}

QWidget *CheckConfigOption::createEditor(QWidget *parent)
{
	m_editor = new QCheckBox();
	m_listener->connect(m_editor, SIGNAL(stateChanged(int)), m_listenerSlot);
	return m_editor;
}

QVariant CheckConfigOption::editorValue() const
{
	return m_editor->checkState() == Qt::Checked;
}

void CheckConfigOption::setEditorValue(const QVariant &value)
{
	setEditorValue(value.toBool());
}

void CheckConfigOption::setLoadedEditorValue(const QVariant &value)
{
	bool val = value.toBool();
	m_editor->setChecked(val);
	setEditorValue(val);
}

void CheckConfigOption::setListener(QObject *listener, const char *listenerSlot)
{
	m_listener = listener;
	m_listenerSlot = listenerSlot;
}

void CheckConfigOption::addOption(ConfigOption *option)
{
	m_options.push_back(option);
}

void CheckConfigOption::removeOption(ConfigOption *option)
{
	m_options.removeAt(m_options.indexOf(option));
}

void CheckConfigOption::setEditorValue(bool value)
{
	for (QList<ConfigOption*>::size_type i = 0, size = m_options.size(); i < size; ++i)
		m_options.at(i)->setEnabled(value);
}
