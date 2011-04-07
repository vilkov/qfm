#ifndef FILESYSTEMENTRY_H_
#define FILESYSTEMENTRY_H_

#include <QDir>
#include <QString>
#include "filesystemitem.h"
#include "../filesysteminfo.h"
#include "../filesystemchangeslist.h"


class FileSystemEntry : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemEntry)

public:
	FileSystemEntry(const FileSystemInfo &info, FileSystemItem *parent = 0) :
		FileSystemItem(parent),
		m_info(info)
	{}
	FileSystemEntry(const Change &change, FileSystemItem *parent = 0) :
		FileSystemItem(parent),
		m_info(change.info())
	{}

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
						return m_info.fileInfo().fileName();
					case Qt::DecorationRole:
						return m_info.icon();
					case Qt::TextAlignmentRole:
						return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
				}
				break;
			}
			case 1:
			{
				switch (role)
				{
					case Qt::EditRole:
					case Qt::DisplayRole:
						if (m_info.fileInfo().isFile())
							return QString::number(m_info.fileInfo().size()).append(QString::fromLatin1(" b"));
						else
							return QString::fromLatin1("<DIR>");
//					case Qt::DecorationRole:
//						return m_info.icon();
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
						return m_info.fileInfo().lastModified();
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

	QIcon icon() const { return m_info.icon(); }
	const QFileInfo &fileInfo() const { return m_info.fileInfo(); }
	QFileInfo &fileInfo() { return m_info.fileInfo(); }

	void update(const FileSystemInfo &info) { m_info = info; }

//	QString path() const { return m_info.fileInfo().absoluteDir().absoluteFilePath(directory()); }

private:
	FileSystemInfo m_info;
};

#endif /* FILESYSTEMENTRY_H_ */
