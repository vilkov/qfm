#include "settingscontainer.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtGui/QDesktopServices>


SETTINGS_NS_BEGIN

Container::Container(const QString &storage) :
	ListOption(NULL),
	m_storage(storage)
{}

Container::~Container()
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
		save(stream);
		stream.writeEndDocument();
	}
}

void Container::load()
{
	QFile file(m_storage);

	if (file.open(QFile::ReadOnly))
	{
		QXmlStreamReader stream(&file);

		if (!stream.atEnd() &&
			stream.readNext() == QXmlStreamReader::StartDocument &&
			stream.readNextStartElement())
		{
			load(stream);
		}
	}
}

void Container::save(QXmlStreamWriter &stream) const
{
	for (size_type i = 0, size = Container::size(); i < size; ++i)
		at(i)->save(stream);
}

void Container::load(QXmlStreamReader &stream)
{
	for (size_type i = 0, size = Container::size(); i < size; ++i)
		at(i)->load(stream);
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
