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
#ifndef IDM_INVALIDINFO_H_
#define IDM_INVALIDINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class InvalidInfo : public IFileInfo, public IFileType
{
public:
	InvalidInfo(const QString &fileName);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual const Location &fileName() const;
	virtual const IFileType *fileType() const;
	virtual const QDateTime &lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual const FileTypeId &id() const;
	virtual const QIcon &icon() const;
	virtual const QString &name() const;
	virtual const QString &description() const;

private:
	Location m_fileName;
	QDateTime m_lastModified;
	FileTypeId m_id;
	QIcon m_icon;
	QString m_name;
	QString m_description;
};

IDM_PLUGIN_NS_END

#endif /* IDM_INVALIDINFO_H_ */
