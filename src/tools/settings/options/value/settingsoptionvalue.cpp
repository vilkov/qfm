#include "settingsoptionvalue.h"


SETTINGS_NS_BEGIN

void OptionValue::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(id(), m_value);
}

void OptionValue::load(QXmlStreamReader &stream)
{
	m_value = stream.readElementText();
}

void OptionValue::loadDefault()
{
	m_value = m_defaultValue;
}

SETTINGS_NS_END
