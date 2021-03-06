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
#include "settingscontainer.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtGui/QDesktopServices>


SETTINGS_NS_BEGIN

Container::Container(const QString &storage) :
	List(QString::fromLatin1("QFM"), NULL),
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
		stream.writeStartElement(id());
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
	m_items.push_back(&option->id(), option);
}

void Container::save(QXmlStreamWriter &stream) const
{
	for (Container::const_iterator i = begin(), end = Container::end(); i != end; ++i)
		(*i)->save(stream);
}

void Container::load(QXmlStreamReader &stream)
{
	Option *option;
	List::Container uninitialized(m_items);

	while (readNextStartElement(stream))
		if (option = uninitialized.value(stream.name()))
		{
			option->load(stream);
			uninitialized.remove(&option->id());
		}

	for (List::Container::const_iterator i = uninitialized.constBegin(), end = uninitialized.constEnd(); i != end; ++i)
		(*i)->loadDefault();
}

void Container::loadDefault()
{
	for (Container::const_iterator i = begin(), end = Container::end(); i != end; ++i)
		(*i)->loadDefault();
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
