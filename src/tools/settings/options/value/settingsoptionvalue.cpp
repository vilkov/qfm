#include "settingsoptionvalue.h"
#include <QtCore/QDebug>


SETTINGS_NS_BEGIN

void OptionValue::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(m_id, m_value);
}

void OptionValue::load(QXmlStreamReader &stream)
{
	if (readNextStartElement(stream, m_id))
		m_value = stream.readElementText();
}

void OptionValue::loadDefault()
{
	m_value = m_defaultValue;
}

SETTINGS_NS_END
