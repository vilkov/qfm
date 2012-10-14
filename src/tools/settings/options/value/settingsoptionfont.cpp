/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
	return QString(font.family()).append(L' ').append(QString::number(font.pointSize()));
}

SETTINGS_NS_END
