#ifndef SETTINGSVALUEOPTION_H_
#define SETTINGSVALUEOPTION_H_

#include "settingsvisibleoption.h"


SETTINGS_NS_BEGIN

class ValueOption : public VisibleOption
{
public:
	ValueOption(qint32 id, const QString &label, const QVariant &defaultValue, Option *parent = 0) :
		VisibleOption(label, parent),
		m_id(id),
		m_defaultValue(defaultValue)
	{}

	qint32 id() const { return m_id; }
	const QVariant &defaultValue() const { return m_defaultValue; }

private:
	qint32 m_id;
	QVariant m_defaultValue;
};

SETTINGS_NS_END

#endif /* SETTINGSVALUEOPTION_H_ */
