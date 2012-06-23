#include "settingscontainer.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtGui/QDesktopServices>


SETTINGS_NS_BEGIN

Container::Container(const QString &storage) :
	List(NULL),
	m_storage(storage)
{}

void Container::save() const
{
	QFile file(m_storage);

	if (file.open(QFile::WriteOnly | QFile::Truncate))
	{
		QXmlStreamWriter stream(&file);
		stream.setCodec(QTextCodec::codecForName("UTF-8"));
		stream.setAutoFormatting(true);

		stream.writeStartDocument(QString::fromLatin1("1.0"));
		stream.writeStartElement(QString::fromLatin1("QFM"));
		save(stream);
		stream.writeEndElement();
		stream.writeEndDocument();
	}
}

void Container::load()
{
	QFile file(m_storage);

	if (file.open(QFile::ReadOnly))
	{
		QXmlStreamReader stream(&file);

		if (readNextStartElement(stream) && stream.name() == QString::fromLatin1("QFM"))
			load(stream);
		else
			loadDefault();
	}
	else
		loadDefault();
}

void Container::manage(Option *option)
{
	m_items.push_back(option);
}

void Container::save(QXmlStreamWriter &stream) const
{
	for (size_type i = 0, size = Container::size(); i < size; ++i)
		at(i)->save(stream);
}

void Container::load(QXmlStreamReader &stream)
{
	for (size_type i = 0, size = Container::size(); i < size; ++i)
		if (readNextStartElement(stream))
			at(i)->load(stream);
		else
		{
			for (; i < size; ++i)
				at(i)->loadDefault();

			break;
		}
}

void Container::loadDefault()
{
	for (size_type i = 0, size = Container::size(); i < size; ++i)
		at(i)->loadDefault();
}

QString Container::storageLocation(const QString &applicationFolder)
{
	QDir dir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

	if (dir.exists(applicationFolder))
		return dir.absoluteFilePath(applicationFolder);
	else
		if (dir.mkpath(dir.absoluteFilePath(applicationFolder)))
			return dir.absoluteFilePath(applicationFolder);

	return QString();
}

SETTINGS_NS_END
