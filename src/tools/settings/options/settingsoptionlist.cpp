#include "settingsoptionlist.h"
#include "../../pointers/pscopedpointer.h"


SETTINGS_NS_BEGIN

OptionList::~OptionList()
{
	clear();
}

void OptionList::clear()
{
	qDeleteAll(m_items);
	m_items.clear();
}

void OptionList::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(id());

	for (const_iterator i = begin(), end = List::end(); i != end; ++i)
		(*i)->save(stream);

	stream.writeEndElement();
}

void OptionList::load(QXmlStreamReader &stream)
{
	PScopedPointer<Option> option;

	for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
		if (token == QXmlStreamReader::EndElement && stream.name() == id())
			break;
		else
			if (token == QXmlStreamReader::StartElement && isSubOptionName(stream.name()))
			{
				option = create();
				option->load(stream);
				add(option.take());
			}
}

void OptionList::loadDefault()
{}

SETTINGS_NS_END
