#include "filesystemcommontools.h"
#include "../../tools/os/osdependent.h"
#include "../../tools/widgets/choosedialog/choosedialog.h"
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QSet>

#if defined(PLATFORMSTL_OS_IS_WINDOWS)
#	include <windows.h>
#elif defined(PLATFORMSTL_OS_IS_UNIX)
#	include <sys/statvfs.h>
#else
#	error OS is unknown!
#endif


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

FILE_SYSTEM_NS_END
