#ifndef SETTINGSVISIBLEOPTION_H_
#define SETTINGSVISIBLEOPTION_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class VisibleOption : public Option
{
public:
	enum Type
	{
		Value,
		List,
		Group,
		Page
	};

public:
	VisibleOption(const QString &label, Option *parent = 0) :
		Option(parent),
		m_label(label)
	{}

	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool value) = 0;

	const QString &label() const { return m_label; }

private:
	QString m_label;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTION_H_ */
