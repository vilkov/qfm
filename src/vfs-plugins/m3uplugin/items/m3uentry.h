/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef M3UENTRY_H_
#define M3UENTRY_H_

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>
#include <vfs/tools/vfs_commontools.h>
#include "m3uitem.h"


M3U_PLUGIN_NS_BEGIN

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

	/* IFileType */
	virtual FileTypeId id() const { return FileTypeId(); }
	virtual QIcon icon() const { return QIcon(); }
	virtual QString name() const { return QString(); }
	virtual QString description() const { return QString(); }

	/* IFileInfo */
	virtual bool isDir() const { return m_info.isDir(); }
	virtual bool isFile() const { return m_info.isFile(); }
	virtual bool isLink() const { return false; }
	virtual bool exists() const { return m_info.exists(); }
	virtual IFileInfo::size_type fileSize() const { return 0; }
	virtual const Location &fileName() const { return m_fileName; }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual const QDateTime &lastModified() const { return m_info.lastModified(); }
	virtual int permissions() const { return 0; }
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
	virtual bool isRoot() const { return false; }

	qint32 length() const { return m_length; }
	const QString &title() const { return m_title; }

private:
	bool m_locked;
	QString m_lockReason;
	QFileInfo m_info;
	Location m_fileName;
	qint32 m_length;
	QString m_title;
};

M3U_PLUGIN_NS_END

#endif /* M3UENTRY_H_ */
