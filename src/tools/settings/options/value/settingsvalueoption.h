#ifndef SETTINGSVALUEOPTION_H_
#define SETTINGSVALUEOPTION_H_

#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class ValueOption : public Option
{
public:
	ValueOption(const QString &id, const QVariant &defaultValue, Option *parent = 0) :
		Option(parent),
		m_id(id),
		m_defaultValue(defaultValue)
	{}

	const QVariant &defaultValue() const { return m_defaultValue; }

private:
	QString m_id;
	QVariant m_defaultValue;
};

SETTINGS_NS_END

#endif /* SETTINGSVALUEOPTION_H_ */
