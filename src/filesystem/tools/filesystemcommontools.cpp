#include "filesystemcommontools.h"
#include "../../tools/os/osdependent.h"
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>

#if defined(PLATFORMSTL_OS_IS_WINDOWS)
#	include <windows.h>
#elif defined(PLATFORMSTL_OS_IS_UNIX)
#	include <sys/statvfs.h>
#else
#	error OS is unknown!
#endif
#include <QtCore/QDebug>


FILE_SYSTEM_NS_BEGIN

QString Tools::humanReadableTime(quint64 msecs)
{
	if (quint64 hours = msecs / (1000 * 60 * 60))
		if (quint64 min = (msecs - hours * 1000 * 60 * 60) / (1000 * 60))
			return QString::number(hours).append(QChar(':')).append(QString::number(min)).append(QString::fromLatin1(" h"));
		else
			return QString::number(hours).append(QString::fromLatin1(" h"));
	else
		if (hours = msecs / (1000 * 60))
			if (quint64 secs = (msecs - hours * 1000 * 60) / (1000))
				return QString::number(hours).append(QChar(':')).append(QString::number(secs)).append(QString::fromLatin1(" m"));
			else
				return QString::number(hours).append(QString::fromLatin1(" m"));
		else
			if (hours = msecs / (1000))
				return QString::number(hours).append(QString::fromLatin1(" s"));
			else
				return QString();
}

QString Tools::humanReadableSize(quint64 size)
{
	if (quint64 gigabytes = size / (1024 * 1024 * 1024))
		if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
			return QString::number(gigabytes).append(QChar(',')).append(QString::number(megabytes)).append(QString::fromLatin1(" Gb"));
		else
			return QString::number(gigabytes).append(QString::fromLatin1(" Gb"));
	else
		if (gigabytes = size / (1024 * 1024))
			if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
				return QString::number(gigabytes).append(QChar(',')).append(QString::number(kilobytes)).append(QString::fromLatin1(" Mb"));
			else
				return QString::number(gigabytes).append(QString::fromLatin1(" Mb"));
		else
			if (gigabytes = size / (1024))
				if (quint64 bytes = (size - gigabytes * 1024) / (1024))
					return QString::number(gigabytes).append(QChar(',')).append(QString::number(bytes)).append(QString::fromLatin1(" Kb"));
				else
					return QString::number(gigabytes).append(QString::fromLatin1(" Kb"));
			else
				return QString::number(size).append(QString::fromLatin1(" b"));
}

QString Tools::humanReadableShortSize(quint64 size)
{
	if (quint64 gigabytes = size / (1024 * 1024 * 1024))
		if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
			return QString::number(gigabytes).append(QChar(',')).append(QString::number(megabytes)).append(QString::fromLatin1(" Gb"));
		else
			return QString::number(gigabytes).append(QString::fromLatin1(" Gb"));
	else
		if (gigabytes = size / (1024 * 1024))
			if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
				return QString::number(gigabytes).append(QChar(',')).append(QString::number(kilobytes).mid(0, 1)).append(QString::fromLatin1(" Mb"));
			else
				return QString::number(gigabytes).append(QString::fromLatin1(" Mb"));
		else
			if (gigabytes = size / (1024))
				if (quint64 bytes = (size - gigabytes * 1024) / (1024))
					return QString::number(gigabytes).append(QChar(',')).append(QString::number(bytes).mid(0, 1)).append(QString::fromLatin1(" Kb"));
				else
					return QString::number(gigabytes).append(QString::fromLatin1(" Kb"));
			else
				return QString::number(size).append(QString::fromLatin1(" b"));
}

quint64 Tools::freeSpace(const QByteArray &utf8AbsoluteFolderPath)
{
	OS_DEPENDENT_CODE
	(
		/* UNIX */
		{
			struct statvfs64 info;

			if (statvfs64(utf8AbsoluteFolderPath, &info) == -1)
				return 0;
			else
				return info.f_bsize * info.f_bfree;
		},

		/* WIN */
		{ return 0; }
	)
}

Tools::DestinationFromPathList::DestinationFromPathList() :
	Tree('/')
{}

QString Tools::DestinationFromPathList::choose(QWidget *parent) const
{
	QString str;
	QStringList list;
	QSet<Item*> items;
	Item *item = const_cast<Tools::DestinationFromPathList*>(this);

	while (item->size() == 1)
		item = item->begin().value();

	if (isDir(str = make(item)))
	{
		items.insert(item);
		list.push_back(str);
	}
	else
	{
		items.insert(item->parent());
		list.push_back(make(item->parent()));
	}

	for (Item::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		if (isDir(str = make(*i)))
		{
			if (!items.contains(*i))
			{
				items.insert(item);
				list.push_back(str);
			}
		}
		else
		{
			if (!items.contains((*i)->parent()))
			{
				items.insert((*i)->parent());
				list.push_back(make((*i)->parent()));
			}
		}

	if (list.isEmpty())
		return QString();
	else
		if (list.size() == 1)
			return list.at(0);
		else
		{
			qDebug() << list;
		}
}

void Tools::DestinationFromPathList::add(const QString &file)
{
	QString str;
	int index1 = 0;
	int index2 = file.size();

	while (index1 < index2 && file.at(index1) == delimeter())
		++index1;

	while ((--index2) >= index1 && file.at(index2) == delimeter());

	if (!(str = file.mid(index1, index2 - index1 + 1)).isEmpty())
		Tree::add(str);
}

bool Tools::DestinationFromPathList::isDir(const QString &file) const
{
	return QFileInfo(file).isDir();
}

FILE_SYSTEM_NS_END
