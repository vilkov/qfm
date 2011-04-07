#ifndef CHECKCONFIGOPTION_H_
#define CHECKCONFIGOPTION_H_

#include <QList>
#include <QCheckBox>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class CheckConfigOption: public ConfigOptionValueDefault
{
public:
	CheckConfigOption(const qint32 &id, const QString &title, ConfigOption *parent = 0) :
		ConfigOptionValueDefault(id, title, QVariant(), parent),
		m_editor(0),
		m_listener(0)
	{}

	virtual bool isEnabled() const { return m_editor->isEnabled(); }
	virtual void setEnabled(bool value) { m_editor->setEnabled(value); }
	virtual QWidget *createEditor(QWidget *parent)
	{
		m_editor = new QCheckBox();
		m_listener->connect(m_editor, SIGNAL(stateChanged(int)), m_listenerSlot);
		return m_editor;
	}
	virtual QVariant editorValue() const { return m_editor->checkState() == Qt::Checked; }
	virtual void setEditorValue(const QVariant &value) { setEditorValue(value.toBool()); }
	virtual void setLoadedEditorValue(const QVariant &value)
	{
		bool val = value.toBool();
		m_editor->setChecked(val);
		setEditorValue(val);
	}

	void setListener(QObject *listener, const char *listenerSlot) { m_listener = listener; m_listenerSlot = listenerSlot; }
	void addOption(ConfigOption *option) { m_options.push_back(option); }
	void removeOption(ConfigOption *option) { m_options.removeAt(m_options.indexOf(option)); }

private:
	inline void setEditorValue(bool value)
	{
		for (QList<ConfigOption*>::size_type i = 0, size = m_options.size(); i < size; ++i)
			m_options.at(i)->setEnabled(value);
	}


private:
	QCheckBox *m_editor;
	QObject *m_listener;
	const char *m_listenerSlot;
	QList<ConfigOption*> m_options;
};

#endif /* CHECKCONFIGOPTION_H_ */
