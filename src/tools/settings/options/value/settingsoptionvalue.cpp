#include "settingsoptionvalue.h"


SETTINGS_NS_BEGIN

void OptionValue::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(m_id, m_value);
}

void OptionValue::load(QXmlStreamReader &stream)
{
	if (stream.name() == m_id)
		m_value = stream.readElementText();
	else
		loadDefault();
}

void OptionValue::loadDefault()
{
	m_value = m_defaultValue;
}

SETTINGS_NS_END
