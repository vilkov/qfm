#include "idmshortformat.h"


IDM_PLUGIN_NS_BEGIN

IdmShortFormat::IdmShortFormat(const QString &format)
{
	m_format.reserve(format.size());

	for (size_type pos = 0; pos < format.size(); ++pos)
		if (format.at(pos) == QChar('$'))
			dollarToken(pos, format);
		else
			m_format.append(format.at(pos));
}

bool IdmShortFormat::isValid() const
{
	return !m_format.isEmpty();
}

void IdmShortFormat::dollarToken(size_type &pos, const QString &source)
{
	if (pos + 1 < source.size())
		if (source.at(pos + 1) == QChar('{'))
			nameToken(++pos, source);
		else
			m_format.append(source.at(pos));
	else
		m_format.append(source.at(pos));
}

void IdmShortFormat::nameToken(size_type &pos, const QString &source)
{
	if (pos + 2 < source.size())
	{
		QString str;
		QString name;

		for (++pos; pos < source.size(); ++pos)
			if (source.at(pos) == QChar('}'))
			{
				if (!name.isEmpty())
				{
					Container::size_type index = m_items.indexOf(name);

					if (index == Container::InvalidIndex)
						m_items.add(name, str = QString::fromLatin1("%").append(QString::number(m_items.size() + 1)));
					else
						str = m_items.at(index);

					m_format.append(str);
				}

				break;
			}
			else
				name.append(source.at(pos));
	}
	else
		m_format.append(source.at(pos));
}

IDM_PLUGIN_NS_END
