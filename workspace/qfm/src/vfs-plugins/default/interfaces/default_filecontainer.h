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
#ifndef DEFAULT_FILECONTAINER_H_
#define DEFAULT_FILECONTAINER_H_

#include "default_filecontainerscanner.h"


DEFAULT_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer, public IApplications
{
public:
	FileContainer(const QString &path);
    FileContainer(const QByteArray &path);

	/* IFileContainer */
	virtual bool isDefault() const;
    virtual const Location &schema() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual const Location &location() const;
	virtual Location location(const IFileInfo *info) const;
	virtual Location location(const QString &fileName) const;

	virtual bool contains(const QString &fileName) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;

    virtual bool remove(const Location &fileName, QString &error) const;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const;

	virtual IFileContainer *open() const;
    virtual IFileContainer *open(const Location &fileName, QString &error) const;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const;
	virtual IFileContainer *create(const QString &fileName, QString &error) const;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;
	virtual const IApplications *applications() const;

	virtual LinkedList user(const IFileType *fileType) const;
	virtual LinkedList system(const IFileType *fileType) const;

protected:
	friend class Info;
    FileContainer(const Location &path);

protected:
	Location m_path;
	FileContainerScanner m_scanner;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILECONTAINER_H_ */
