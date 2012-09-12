#include "settingsscope.h"


SETTINGS_NS_BEGIN

void Scope::manage(Option *option)
{
	m_items.push_back(&option->id(), option);
}

void Scope::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(id());

	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		(*i)->save(stream);

	stream.writeEndElement();
}

void Scope::load(QXmlStreamReader &stream)
{
	Option *option;
	List::Container uninitialized(m_items);

	while (readNextStartElement(stream))
		if (option = uninitialized.value(stream.name()))
		{
			option->load(stream);
			uninitialized.remove(&option->id());
		}

	if (!uninitialized.isEmpty())
		for (List::Container::const_iterator i = uninitialized.constBegin(), end = uninitialized.constEnd(); i != end; ++i)
			(*i)->loadDefault();
}

void Scope::loadDefault()
{
	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		(*i)->loadDefault();
}

SETTINGS_NS_END
