#include "settingsoptionlist.h"
#include "../../pointers/pscopedpointer.h"
#include <QtCore/QDebug>


SETTINGS_NS_BEGIN

OptionList::~OptionList()
{
	clear();
}

void OptionList::clear()
{
	qDeleteAll(m_items);
}

void OptionList::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(m_id);

	for (size_type i = 0, size = List::size(); i < size; ++i)
		at(i)->save(stream);

	stream.writeEndElement();
}

void OptionList::load(QXmlStreamReader &stream)
{
	if (readNextStartElement(stream, m_id))
	{
		qDebug() << stream.name();
		PScopedPointer<Option> option;

		for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
			if (token == QXmlStreamReader::EndElement && stream.name() == m_id)
				break;
			else
			{
				qDebug() << stream.name();
				option = create();
				option->load(stream);
				add(option.take());
			}
	}
}

void OptionList::loadDefault()
{}

SETTINGS_NS_END
