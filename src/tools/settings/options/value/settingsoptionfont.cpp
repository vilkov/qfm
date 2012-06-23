#include "settingsoptionfont.h"

#include <QtGui/QFont>


SETTINGS_NS_BEGIN

OptionFont::OptionFont(const QString &id, Option *parent, const QFont &defaultValue) :
	Scope(id, parent),
    m_defaultValue(defaultValue),
	m_family(QString::fromLatin1("Family"), this),
	m_styleName(QString::fromLatin1("styleName"), this),
	m_pointSize(QString::fromLatin1("pixelSize"), this),
	m_underline(QString::fromLatin1("underline"), this),
	m_strikeOut(QString::fromLatin1("strikeOut"), this)
{
	manage(&m_family);
	manage(&m_styleName);
	manage(&m_pointSize);
	manage(&m_underline);
	manage(&m_strikeOut);
}

QFont OptionFont::font() const
{
	QFont res(m_family.value());

	res.setStyleName(m_styleName.value());
	res.setPointSize(m_pointSize.value().toInt());
	res.setUnderline(m_underline.value().toInt());
	res.setStrikeOut(m_strikeOut.value().toInt());

	return res;
}

void OptionFont::setFont(const QFont &font)
{
	m_family.setValue(font.family());
	m_styleName.setValue(font.styleName());
	m_pointSize.setValue(QString::number(font.pointSize()));
	m_underline.setValue(QString::number(font.underline()));
	m_strikeOut.setValue(QString::number(font.strikeOut()));
}

void OptionFont::loadDefault()
{
	setFont(m_defaultValue);
}

QString OptionFont::toString(const QFont &font)
{
	return QString(font.family()).append(' ').append(QString::number(font.pointSize()));
}

SETTINGS_NS_END
