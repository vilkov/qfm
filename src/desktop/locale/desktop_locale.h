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
