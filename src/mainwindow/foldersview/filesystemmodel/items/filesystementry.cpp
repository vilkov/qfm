#include "filesystementry.h"
#include "../visitor/filesystemmodelvisitor.h"


FileSystemEntry::FileSystemEntry(const FileSystemInfo &info, FileSystemItem *parent) :
	FileSystemItem(parent),
	m_locked(false),
	m_info(info)
{}

FileSystemEntry::FileSystemEntry(const Change &change, FileSystemItem *parent) :
	FileSystemItem(parent),
	m_locked(false),
	m_info(change.info())
{}

QVariant FileSystemEntry::data(qint32 column, qint32 role) const
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
						if (m_fileSize.isNull())
							return QString::fromLatin1("<DIR>");
						else
							return QString::number(m_fileSize.toULongLong()).append(QString::fromLatin1(" b"));
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

void FileSystemEntry::accept(FileSystemModelVisitor *visitor) const
{
	visitor->visit(this);
}
