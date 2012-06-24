#include "settingsdecoration.h"


SETTINGS_NS_BEGIN

Decoration::Decoration(Option *parent) :
	Option(parent)
{}

void Decoration::restoreDefault()
{}

bool Decoration::accept()
{
	return true;
}

void Decoration::reject()
{}

void Decoration::save(QXmlStreamWriter &stream) const
{
	static const QString id = QString::fromLatin1("Decoration");
	stream.writeStartElement(id);
	stream.writeEndElement();
}

void Decoration::load(QXmlStreamReader &stream)
{}

void Decoration::loadDefault()
{}

SETTINGS_NS_END
