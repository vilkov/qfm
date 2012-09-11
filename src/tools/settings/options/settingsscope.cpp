#include "settingsscope.h"
#include <QtCore/QSet>


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
	QSet<Option *> loaded;

	while (readNextStartElement(stream))
		if (option = m_items.value(stream.name()))
		{
			option->load(stream);
			loaded.insert(option);
		}

	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		if (!loaded.contains(*i))
			(*i)->loadDefault();
}

void Scope::loadDefault()
{
	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		(*i)->loadDefault();
}

SETTINGS_NS_END
