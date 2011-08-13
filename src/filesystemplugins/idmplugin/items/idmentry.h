#ifndef IDMENTRY_H_
#define IDMENTRY_H_

#include "idminfoitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntry : public IdmInfoItem
{
	Q_DECLARE_TR_FUNCTIONS(IdmEntry)

public:
	IdmEntry(const QFileInfo &info, qint32 length, const QString &title, IdmItem *parent = 0) :
		IdmInfoItem(info, parent),
		m_locked(false),
		m_length(length),
		m_title(title)
	{}

	/* IdmItem */
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
						if (exists())
							return absoluteFilePath();
						else
							return tr("File does not exists - ").append(absoluteFilePath());
				}
				break;
			}
			case 1:
			{
				switch (role)
				{
					case Qt::EditRole:
					case Qt::DisplayRole:
						return Tools::humanReadableTime(m_length);
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

	qint32 length() const { return m_length; }
	const QString &title() const { return m_title; }

private:
	bool m_locked;
	QString m_lockReason;
	qint32 m_length;
	QString m_title;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTRY_H_ */
