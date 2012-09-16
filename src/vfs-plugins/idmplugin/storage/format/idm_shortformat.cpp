#include "idm_shortformat.h"


IDM_PLUGIN_NS_BEGIN

IdmShortFormat::IdmShortFormat(const QString &format) :
	m_format(format)
{
	QString token;
	token.reserve(m_format.size());

	for (size_type pos = 0; pos < m_format.size(); ++pos)
		if (m_format.at(pos) == QChar(L'$'))
			dollarToken(pos, token, m_format);
		else
			token.append(m_format.at(pos));

	if (!token.isEmpty())
	{
		token.truncate(token.size());
		m_items.push_back(Token(Token::Text, token));
	}
}

bool IdmShortFormat::isValid() const
{
	return !m_items.isEmpty();
}

void IdmShortFormat::dollarToken(size_type &pos, QString &token, const QString &source)
{
	if (pos + 1 < source.size())
		if (source.at(pos + 1) == QChar(L'{'))
			nameToken(++pos, token, source);
		else
			token.append(source.at(pos));
	else
		token.append(source.at(pos));
}

void IdmShortFormat::nameToken(size_type &pos, QString &token, const QString &source)
{
	if (pos + 2 < source.size())
	{
		QString name;

		for (++pos; pos < source.size(); ++pos)
			if (source.at(pos) == QChar(L'}'))
			{
				if (!name.isEmpty())
				{
					token.truncate(token.size());
					m_items.push_back(Token(Token::Text, token));
					m_items.push_back(Token(Token::Property, name));

					token.clear();
					token.reserve(source.size());
				}

				break;
			}
			else
				name.append(source.at(pos));
	}
	else
		token.append(source.at(pos));
}

IDM_PLUGIN_NS_END
