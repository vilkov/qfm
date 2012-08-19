#include "../desktop_locale.h"
#include "../../../tools/templates/templates_statemachine.h"

#include <stdlib.h>


DESKTOP_NS_BEGIN
static Locale *instance;


class Parser
{
public:
	enum State
	{
		Lang,
		Country,
		Modifier,
		Encoding,
		Stoped
	};

public:
	Parser(const char *string) :
		m_valid(false)
	{
		typedef RULE(Lang,     '_', Country,  Parser, lang)        Rule1;
		typedef RULE(Country,  '@', Modifier, Parser, country)     Rule2;
		typedef RULE(Modifier, '.', Encoding, Parser, modifier)    Rule3;
		typedef RULE(Country,  '.', Encoding, Parser, country)     Rule4;
		typedef RULE(Encoding,   0, Stoped,   Parser, encoding)    Rule5;
		typedef LOKI_TYPELIST_5(Rule1, Rule2, Rule3, Rule4, Rule5) Rules;

		m_valid = ::Tools::Templates::StateMachine<Rules>(this).process(string) == Stoped;
	}

private:
	void lang(const char *string, int size)
	{
		m_lang = QByteArray(string, size);
	}

	void country(const char *string, int size)
	{
		m_country = QByteArray(string, size);
	}

	void modifier(const char *string, int size)
	{
		m_modifier = QByteArray(string, size);
	}

	void encoding(const char *string, int size)
	{
		m_encoding = QByteArray(string, size);
	}

private:
	friend class Locale;
	bool m_valid;
	QByteArray m_lang;
	QByteArray m_country;
	QByteArray m_modifier;
	QByteArray m_encoding;
};


Locale::Locale() :
	m_codec(NULL)
{
	Q_ASSERT(instance == NULL);

	if (const char *locale = getenv("LANG"))
	{
		Parser parser(locale);

		if (parser.m_valid)
		{
			m_lang = parser.m_lang;
			m_country = parser.m_country;
			m_modifier = parser.m_modifier;

			if ((m_codec = QTextCodec::codecForName(parser.m_encoding)) == NULL)
				m_codec = QTextCodec::codecForName("UTF-8");
		}
		else
			setDefaultLocale();
	}
	else
		setDefaultLocale();

	instance = this;
}

Locale *Locale::current()
{
	Q_ASSERT(instance);
	return instance;
}

void Locale::setDefaultLocale()
{
	m_lang = "en";
	m_country = "GB";
	m_codec = QTextCodec::codecForName("UTF-8");
}

DESKTOP_NS_END
