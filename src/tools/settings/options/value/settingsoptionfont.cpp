#include "settingsoptionfont.h"

#include <QtGui/QFont>


SETTINGS_NS_BEGIN

OptionFont::OptionFont(const QString &id, Option *parent) :
	Scope(id, parent),
	m_family(QString::fromLatin1("Family"), this)
{}

QFont OptionFont::font() const
{
	return QFont();
}

void OptionFont::setFont(const QFont &font)
{

}

SETTINGS_NS_END
