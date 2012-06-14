#include "settingsscope.h"


SETTINGS_NS_BEGIN

Scope::~Scope()
{
	qDeleteAll(m_items);
}

void Scope::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(id());

	for (size_type i = 0, size = Scope::size(); i < size; ++i)
		at(i)->save(stream);

	stream.writeEndElement();
}

void Scope::load(QXmlStreamReader &stream)
{
	if (stream.name() == id())
		for (size_type i = 0, size = Scope::size(); i < size; ++i)
			at(i)->load(stream);
}

SETTINGS_NS_END
