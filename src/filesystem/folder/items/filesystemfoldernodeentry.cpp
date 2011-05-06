#include "filesystemfoldernodeentry.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderNodeEntry::FolderNodeEntry(const Info &info) :
	m_locked(false),
	m_info(info)
{}

QVariant FolderNodeEntry::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info.fileName();
				case Qt::DecorationRole:
					if (m_locked)
						return Application::style()->standardIcon(QStyle::SP_BrowserReload);
					else
						return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					if (m_locked)
						return m_lockReason;
					else
						break;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					if (m_info.isFile())
						return humanReadableSize(m_info.size());
					else
						if (m_totalSize.isNull())
							return QString::fromLatin1("<DIR>");
						else
							return humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info.lastModified();
//					case Qt::DecorationRole:
//						return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
			}
			break;
		}
	}

	return QVariant();
}

void FolderNodeEntry::lock(const QString &reason, quint64 totalSize)
{
	m_locked = true;
	m_lockReason = reason;
	m_totalSize = totalSize;
	m_doneSize = 0;
	m_timeElapsed = 0;
}

void FolderNodeEntry::lock(const QString &reason)
{
	m_locked = true;
	m_lockReason = reason;
}

void FolderNodeEntry::unlock()
{
	m_locked = false;
	m_lockReason.clear();
	m_doneSize.clear();
	m_timeElapsed.clear();
}

QString FolderNodeEntry::humanReadableTime(quint64 msecs)
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

QString FolderNodeEntry::humanReadableSize(quint64 size)
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

QString FolderNodeEntry::humanReadableShortSize(quint64 size)
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
