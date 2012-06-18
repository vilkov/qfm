#include "settingsoptionvaluelist.h"
#include <QtCore/QDebug>


SETTINGS_NS_BEGIN

void OptionValueList::save(QXmlStreamWriter &stream) const
{
	static const QString id = QString::fromLatin1("Value");

	stream.writeStartElement(m_id);

	for (size_type i = 0, size = OptionValueList::size(); i < size; ++i)
		stream.writeTextElement(id, at(i));

	stream.writeEndElement();
}

void OptionValueList::load(QXmlStreamReader &stream)
{
	qDebug() << stream.name();

	if (readNextStartElement(stream, m_id))
	{
		qDebug() << stream.name();
		Option *option;

		for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
			if (token == QXmlStreamReader::StartElement)
			{
				QString s;
				m_items.push_back(s = stream.readElementText());
				qDebug() << s;
			}
			else
				if (token == QXmlStreamReader::EndElement && stream.name() == m_id)
					break;
	}
}

void OptionValueList::loadDefault()
{}

SETTINGS_NS_END
