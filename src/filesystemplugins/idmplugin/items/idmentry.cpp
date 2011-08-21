#include "idmentry.h"


FILE_SYSTEM_NS_BEGIN

IdmEntry::IdmEntry(const Info &info, qint32 length, const QString &title, IdmItem *parent) :
	IdmInfoItem(info, parent),
	m_locked(false),
	m_length(length),
	m_title(title)
{}

QVariant IdmEntry::data(qint32 column, qint32 role) const
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

FILE_SYSTEM_NS_END
