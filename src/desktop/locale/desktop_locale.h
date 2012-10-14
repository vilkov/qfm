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
#ifndef DESKTOP_LOCALE_H_
#define DESKTOP_LOCALE_H_

#include <QtCore/QByteArray>
#include <QtCore/QTextCodec>
#include "../desktop_ns.h"


DESKTOP_NS_BEGIN

class Locale
{
public:
	Locale();

	static Locale *current();

	const QByteArray &lang() const { return m_lang; }
	const QByteArray &country() const { return m_country; }
	const QByteArray &modifier() const { return m_modifier; }
	const QTextCodec *codec() const { return m_codec; }

private:
	void setDefaultLocale();

private:
	QByteArray m_lang;
	QByteArray m_country;
	QByteArray m_modifier;
	QTextCodec *m_codec;
};

DESKTOP_NS_END

#endif /* DESKTOP_LOCALE_H_ */
