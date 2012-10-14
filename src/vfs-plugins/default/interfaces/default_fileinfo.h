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
#ifndef DEFAULT_FILEINFO_H_
#define DEFAULT_FILEINFO_H_

#include <QtCore/QTextCodec>
#include <vfs/filetypeinfo/vfs_filetypeinfo.h>
#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../default_ns.h"


VFS_NS(class IFileContainer)


DEFAULT_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	template <int I> struct int_to_type {enum { value = I };};
	typedef int_to_type<0> None;
	typedef int_to_type<1> Refresh;

public:
    explicit Info();
    explicit Info(const Info &other);

    template <typename T>
    explicit Info(const QByteArray &filePath, T = T());

    explicit Info(const QByteArray &filePath, const IFileContainer *container);

    template <typename T>
    explicit Info(const Info &other, T = T());

    explicit Info(const Info &other, const IFileContainer *container);

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

public:
	static QTextCodec *codec();
	static FileTypeInfo fileTypeInfoFromFileName(const QByteArray &fileName, bool isDir);

#ifdef Q_OS_UNIX
	static QIcon fileTypeIcon(const char *mimeType, int size);
#endif

	void refresh();
	void identify(const IFileContainer *container);
	bool isRoot() const { return m_isRoot; }
	bool exists() const { return m_info.exists; }

private:
	struct FileInfo
	{
		FileInfo() :
			isDir(false),
			isFile(false),
			isLink(false),
			exists(false),
			permissions(0),
			size(0)
		{}

	    bool isDir;
	    bool isFile;
	    bool isLink;
	    bool exists;
	    int permissions;
	    qint64 size;
	    QDateTime lastModified;
	    FileTypeInfo type;
	};

private:
    bool m_isRoot;
    QByteArray m_filePath;
    Location m_fileName;
    FileInfo m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILEINFO_H_ */
