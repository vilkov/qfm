#include "settingsscope.h"
#include <QtCore/QDebug>


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
	qDebug() << stream.name();

	if (readNextStartElement(stream, m_id))
	{
		qDebug() << stream.name();

		for (size_type i = 0, size = Scope::size(); i < size; ++i)
			at(i)->load(stream);
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
