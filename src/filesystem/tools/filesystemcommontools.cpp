#include "filesystemcommontools.h"


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

FILE_SYSTEM_NS_END