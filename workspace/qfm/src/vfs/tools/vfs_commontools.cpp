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
#include "vfs_commontools.h"

#include <platform/platform.h>
#include <tools/widgets/choosedialog/choosedialog.h>

#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QSet>

#if PLATFORM_OS(WINDOWS)
#	include <windows.h>
#elif PLATFORM_OS(UNIX)
#	include <sys/statvfs.h>
#else
#	error OS is unknown!
#endif


VFS_NS_BEGIN

QString Tools::humanReadableTime(quint64 msecs)
{
	if (quint64 hours = msecs / (1000 * 60 * 60))
		if (quint64 min = (msecs - hours * 1000 * 60 * 60) / (1000 * 60))
			return QString::number(hours).append(QChar(L':')).append(QString::number(min)).append(QLatin1String(" h"));
		else
			return QString::number(hours).append(QLatin1String(" h"));
	else
		if (hours = msecs / (1000 * 60))
			if (quint64 secs = (msecs - hours * 1000 * 60) / (1000))
				return QString::number(hours).append(QChar(L':')).append(QString::number(secs)).append(QLatin1String(" m"));
			else
				return QString::number(hours).append(QLatin1String(" m"));
		else
			if (hours = msecs / (1000))
				return QString::number(hours).append(QLatin1String(" s"));
			else
                return QString::number(hours).append(QLatin1String(" ms"));
}

QString Tools::humanReadableSize(quint64 size)
{
	if (quint64 gigabytes = size / (1024 * 1024 * 1024))
		if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
			return QString::number(gigabytes).append(QChar(L',')).append(QString::number(megabytes)).append(QLatin1String(" Gb"));
		else
			return QString::number(gigabytes).append(QLatin1String(" Gb"));
	else
		if (gigabytes = size / (1024 * 1024))
			if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
				return QString::number(gigabytes).append(QChar(L',')).append(QString::number(kilobytes)).append(QLatin1String(" Mb"));
			else
				return QString::number(gigabytes).append(QLatin1String(" Mb"));
		else
			if (gigabytes = size / (1024))
				if (quint64 bytes = (size - gigabytes * 1024) / (1024))
					return QString::number(gigabytes).append(QChar(L',')).append(QString::number(bytes)).append(QLatin1String(" Kb"));
				else
					return QString::number(gigabytes).append(QLatin1String(" Kb"));
			else
				return QString::number(size).append(QLatin1String(" b"));
}

QString Tools::humanReadableShortSize(quint64 size)
{
	if (quint64 gigabytes = size / (1024 * 1024 * 1024))
		if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
			return QString::number(gigabytes).append(QChar(L',')).append(QString::number(megabytes)).append(QLatin1String(" Gb"));
		else
			return QString::number(gigabytes).append(QLatin1String(" Gb"));
	else
		if (gigabytes = size / (1024 * 1024))
			if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
				return QString::number(gigabytes).append(QChar(L',')).append(QString::number(kilobytes).mid(0, 1)).append(QLatin1String(" Mb"));
			else
				return QString::number(gigabytes).append(QLatin1String(" Mb"));
		else
			if (gigabytes = size / (1024))
				if (quint64 bytes = (size - gigabytes * 1024) / (1024))
					return QString::number(gigabytes).append(QChar(L',')).append(QString::number(bytes).mid(0, 1)).append(QLatin1String(" Kb"));
				else
					return QString::number(gigabytes).append(QLatin1String(" Kb"));
			else
				return QString::number(size).append(QLatin1String(" b"));
}

Tools::DestinationFromPathList::DestinationFromPathList() :
	Tree(L'/')
{}

QString Tools::DestinationFromPathList::choose(const QString &title, QWidget *parent) const
{
	QString str;
	QSet<Item*> items;
	ChooseDialog::List list;
	Item *item = const_cast<Tools::DestinationFromPathList*>(this);

	while (item->size() == 1)
		item = item->begin().value();

	items.insert(item);
	list.push_back(ChooseDialog::List::value_type(make(item), item));

	for (Item::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		if (!items.contains(*i))
		{
			items.insert(*i);
			list.push_back(ChooseDialog::List::value_type(make(*i), *i));
		}

	if (list.isEmpty())
		return QString();
	else
		if (list.size() == 1)
			return list.at(0).first;
		else
		{
			ChooseDialog dialog(title, list, parent);

			if (dialog.exec() == ChooseDialog::Accepted)
				return make(static_cast<Item*>(dialog.value()));
			else
				return QString();
		}
}

void Tools::DestinationFromPathList::add(const QString &file, qint32 excludeLastEntries)
{
	QString str;
	int index1 = -1;
	int index2 = file.size();

	while ((++index1) < index2 && file.at(index1) == delimeter());

	while ((--index2) >= index1 && file.at(index2) == delimeter());

	if (!(str = file.mid(index1, index2 - index1 + 1)).isEmpty())
	{
		for (
				index1 = str.lastIndexOf(delimeter(), -1), --excludeLastEntries;
				index1 != -1 && excludeLastEntries > 0;
				index1 = str.lastIndexOf(delimeter(), index1 - str.size() - 1), --excludeLastEntries
			);

		if (index1 != -1 && !(str = str.mid(0, index1)).isEmpty())
			Tree::add(str);
	}
}

void Tools::DestinationFromPathList::add(const QString &file)
{
	QString str;
	int index1 = -1;
	int index2 = file.size();

	while ((++index1) < index2 && file.at(index1) == delimeter());

	while ((--index2) >= index1 && file.at(index2) == delimeter());

	if (!(str = file.mid(index1, index2 - index1 + 1)).isEmpty())
		Tree::add(str);
}

VFS_NS_END
