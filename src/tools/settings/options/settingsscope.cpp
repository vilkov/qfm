#include "settingsscope.h"


SETTINGS_NS_BEGIN

void Scope::manage(Option *option)
{
	m_items.push_back(option);
}

void Scope::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(m_id);

	for (size_type i = 0, size = Scope::size(); i < size; ++i)
		at(i)->save(stream);

	stream.writeEndElement();
}

void Scope::load(QXmlStreamReader &stream)
{
	if (stream.name() == m_id)
		for (size_type i = 0, size = Scope::size(); i < size; ++i)
			if (readNextStartElement(stream))
				at(i)->load(stream);
			else
			{
				for (; i < size; ++i)
					at(i)->loadDefault();

				break;
			}
	else
		loadDefault();
}

void Scope::loadDefault()
{
	for (size_type i = 0, size = Scope::size(); i < size; ++i)
		at(i)->loadDefault();
}

SETTINGS_NS_END
