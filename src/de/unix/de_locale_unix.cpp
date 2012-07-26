#include "../de_locale.h"

#include <stdlib.h>
#include "../../tools/templates/templates_statemachine.h"


DE_NS_BEGIN

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
	Parser(const char *string)
	{
		typedef RULE(Lang,     '_', Country,  Parser, lang)     Rule1;
		typedef RULE(Country,  '@', Modifier, Parser, country)  Rule2;
		typedef RULE(Modifier, '.', Encoding, Parser, modifier) Rule3;
		typedef RULE(Country,  '.', Encoding, Parser, country)  Rule4;
		typedef RULE(Encoding,   0, Stoped,   Parser, encoding) Rule5;
		typedef TYPELlST_5(Rule1, Rule2, Rule3, Rule4, Rule5)   Rules;

		Tools::Templates::StateMachine<Rules>(this).process(string);
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
	QByteArray m_lang;
	QByteArray m_country;
	QByteArray m_modifier;
	QByteArray m_encoding;
};


Locale::Locale() :
	m_codec(NULL)
{
	if (const char *locale = getenv("LANG"))
	{
		Parser parser(locale);

		m_lang = parser.m_lang;
		m_country = parser.m_country;
		m_modifier = parser.m_modifier;

		if ((m_codec = QTextCodec::codecForName(parser.m_encoding)) == NULL)
			m_codec = QTextCodec::codecForName("UTF-8");
	}
	else
	{
		m_lang = "en";
		m_country = "GB";
		m_codec = QTextCodec::codecForName("UTF-8");
	}
}

DE_NS_END
