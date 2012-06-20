#include "settingsvisibleoptionhotkey.h"


SETTINGS_NS_BEGIN

VisibleOptionHotKey::VisibleOptionHotKey(const QString &label, const QString &context, const QString &id, Option *parent, int modifiers, int key) :
	Scope(id, parent),
	m_label(label),
	m_context(context),
	m_modifiers(QString::fromLatin1("Modifiers"), this, QString::number(modifiers)),
	m_key(QString::fromLatin1("Key"), this, QString::number(key))
{
	manage(&m_modifiers);
	manage(&m_key);
}

SETTINGS_NS_END
