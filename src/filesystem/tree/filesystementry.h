#ifndef FILESYSTEMENTRY_H_
#define FILESYSTEMENTRY_H_

#include <QtCore/QVariant>
#include "filesystemitem.h"


class FileSystemEntry : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemEntry)

public:
	FileSystemEntry(FileSystemItem *parent = 0) :
		FileSystemItem(parent)
	{}

	virtual QString fileName() const = 0;
	virtual QString absolutePath() const = 0;
	virtual QString absoluteFilePath() const = 0;
	virtual QVariant data(qint32 column, qint32 role) const = 0;

public:
	static QString humanReadableTime(quint64 msecs)
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

	static QString humanReadableSize(quint64 size)
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

	static QString humanReadableShortSize(quint64 size)
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
};

#endif /* FILESYSTEMENTRY_H_ */
