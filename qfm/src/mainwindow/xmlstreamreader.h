#ifndef XMLSTREAMREADER_H_
#define XMLSTREAMREADER_H_

#include <QtXml/QXmlStreamReader>


class XmlStreamReader
{
public:
	XmlStreamReader(QXmlStreamReader &stream) :
		m_stream(stream)
	{}

	bool atEnd() const { return m_stream.atEnd(); }
	QStringRef name() const { return m_stream.name(); }
	QStringRef text() const { return m_stream.text(); }

	QXmlStreamReader::TokenType readNext() { return m_stream.readNext(); }
	bool readNextStart() { return m_stream.readNextStartElement(); }
	bool readNext(const QString &name)
	{
		if (readNext() && m_stream.name() == name)
			return true;
		else
			return false;
	}

private:
	QXmlStreamReader &m_stream;
};

#endif /* XMLSTREAMREADER_H_ */
