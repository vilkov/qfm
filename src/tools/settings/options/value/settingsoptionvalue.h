#ifndef SETTINGSOPTIONVALUE_H_
#define SETTINGSOPTIONVALUE_H_

#include <QtCore/QString>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class OptionValue : public Option
{
public:
	OptionValue(const Id &id, Option *parent, const QString &defaultValue = QString()) :
		Option(id, parent),
		m_defaultValue(defaultValue)
	{}

	const QString &value() const { return m_value; }
	void setValue(const QString &value) { m_value = value; }

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
    const QString &defaultValue() const { return m_defaultValue; }

private:
    QString m_value;
    QString m_defaultValue;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONVALUE_H_ */
