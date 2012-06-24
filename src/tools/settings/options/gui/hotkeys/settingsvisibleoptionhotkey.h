#ifndef SETTINGSVISIBLEOPTIONHOTKEY_H_
#define SETTINGSVISIBLEOPTIONHOTKEY_H_

#include "../../settingsscope.h"
#include "../../value/settingsoptionvalue.h"


SETTINGS_NS_BEGIN

class VisibleOptionHotKey : public Scope
{
public:
	VisibleOptionHotKey(const QString &label, const QString &context, const QString &id, Option *parent, int modifiers = Qt::NoModifier, int key = 0);

	const QString &label() const { return m_label; }
	const QString &context() const { return m_context; }

	int modifiers() const { return m_modifiers.value().toInt(); }
	void setModifiers(int value) { m_modifiers.setValue(QString::number(value)); }

	int key() const { return m_key.value().toInt(); }
	void setKey(int value) { m_key.setValue(QString::number(value)); }

private:
	QString m_label;
	QString m_context;
	OptionValue m_modifiers;
	OptionValue m_key;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONHOTKEY_H_ */