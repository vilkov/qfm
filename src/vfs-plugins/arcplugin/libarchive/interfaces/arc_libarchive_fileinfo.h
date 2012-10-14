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
#ifndef ARC_LIBARCHIVE_FILEINFO_H_
#define ARC_LIBARCHIVE_FILEINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../arc_libarchive_ns.h"


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	struct Data
	{
		QByteArray path;
		Location fileName;
		size_type fileSize;
		QDateTime lastModified;
	};

public:
	Info();
	Info(const Data &data, bool isDir);

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

	const QByteArray &path() const { return m_data.path; }

	static Location location(const QByteArray &location);

private:
	Data m_data;
	bool m_isDir;
	FileTypeInfo m_fileTypeInfo;
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_FILEINFO_H_ */
