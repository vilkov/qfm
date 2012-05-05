#ifndef CONFIGOPTION_H_
#define CONFIGOPTION_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../settings_ns.h"
#include "../../pointers/pscopedpointer.h"


SETTINGS_NS_BEGIN

class ConfigOption
{
public:
	enum Type
	{
		Value,
		Group
	};

public:
	ConfigOption(const QString &title, ConfigOption *parent = 0) :
		m_title(title),
		m_parent(parent)
	{}
	virtual ~ConfigOption() {}

	virtual Type type() const = 0;
	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool value) = 0;
	const QString &title() const { return m_title; }
	ConfigOption *parent() const { return m_parent; }

private:
	QString m_title;
	ConfigOption *m_parent;
};

SETTINGS_NS_END

#endif /* CONFIGOPTION_H_ */
