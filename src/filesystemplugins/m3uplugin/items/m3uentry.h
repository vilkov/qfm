#ifndef M3UENTRY_H_
#define M3UENTRY_H_

#include <QtCore/QFileInfo>
#include "m3uitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

class M3uEntry : public M3uItem
{
	Q_DECLARE_TR_FUNCTIONS(M3uEntry)

public:
	M3uEntry(const QFileInfo &info, qint32 length, const QString &title) :
		m_locked(false),
		m_info(info),
		m_length(length),
		m_title(title)
	{}

	/* IFileInfo */
	virtual bool isDir() const { return m_info.isDir(); }
	virtual bool isFile() const { return m_info.isFile(); }
	virtual bool exists() const { return m_info.exists(); }
	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual QDateTime lastModified() const { return m_info.lastModified(); }
	virtual void refresh() {}

	/* M3uItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		switch (column)
		{
			case 0:
			{
				switch (role)
				{
					case Qt::EditRole:
					case Qt::DisplayRole:
						return m_title;
//					case Qt::DecorationRole:
//						if (m_locked)
//							return Application::style()->standardIcon(QStyle::SP_BrowserReload);
//						else
//							return m_info.icon();
					case Qt::TextAlignmentRole:
						return Qt::AlignLeft;
					case Qt::ToolTipRole:
						if (m_info.exists())
							return m_info.absoluteFilePath();
						else
							return tr("File does not exists - ").append(m_info.absoluteFilePath());
				}
				break;
			}
			case 1:
			{
				switch (role)
				{
					case Qt::EditRole:
					case Qt::DisplayRole:
						return humanReadableTime(m_length);
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
	virtual bool isRoot() const { return false; }

	qint32 length() const { return m_length; }
	const QString &title() const { return m_title; }

	static QString humanReadableTime(quint64 secs)
	{
		if (quint64 hours = secs / (60 * 60))
			if (quint64 min = (secs - hours * 60 * 60) / (60))
				return QString::number(hours).append(QChar(':')).append(QString::number(min)).append(QString::fromLatin1(" h"));
			else
				return QString::number(hours).append(QString::fromLatin1(" h"));
		else
			if (hours = secs / (60))
				if (quint64 s = (secs - hours * 60))
					return QString::number(hours).append(QChar(':')).append(QString::number(s)).append(QString::fromLatin1(" m"));
				else
					return QString::number(hours).append(QString::fromLatin1(" m"));
			else
				return QString::number(secs).append(QString::fromLatin1(" s"));
	}

private:
	bool m_locked;
	QString m_lockReason;
	QFileInfo m_info;
	qint32 m_length;
	QString m_title;
};

FILE_SYSTEM_NS_END

#endif /* M3UENTRY_H_ */
