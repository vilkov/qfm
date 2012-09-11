#include "settingsoptionvaluelist.h"


SETTINGS_NS_BEGIN

void OptionValueList::save(QXmlStreamWriter &stream) const
{
	static const QString id = QString::fromLatin1("Value");

	stream.writeStartElement(OptionValueList::id());

	for (size_type i = 0, size = OptionValueList::size(); i < size; ++i)
		stream.writeTextElement(id, at(i));

	stream.writeEndElement();
}

void OptionValueList::load(QXmlStreamReader &stream)
{
	for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
		if (token == QXmlStreamReader::StartElement)
			m_items.push_back(stream.readElementText());
		else
			if (token == QXmlStreamReader::EndElement && stream.name() == id())
				break;
}

void OptionValueList::loadDefault()
{}

SETTINGS_NS_END
