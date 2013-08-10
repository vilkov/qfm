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
#ifndef ARC_LIBARCHIVE_FILECONTAINER_H_
#define ARC_LIBARCHIVE_FILECONTAINER_H_

#include <QtCore/QSharedData>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "arc_libarchive_scanner.h"


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class FileContainer : public IFileContainer
{
public:
	static IFileContainer *create(const IFileContainer *container, const IFileInfo *file, QString &error);
	static QString extractDirectoryName(const IFileInfo *file);
	static QString extractArchivedFileName(const IFileInfo *file);

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

private:
	FileContainer(IFileContainer::Holder &container, IFileAccessor::Holder &file, const Location &location);

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container, IFileAccessor::Holder &file, const Location &location) :
			container(container.take()),
			location(location),
			scanner(this->container.data(), file)
		{}

		IFileContainer::Holder container;
		Location location;
		Scanner scanner;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_FILECONTAINER_H_ */
