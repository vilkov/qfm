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
#ifndef DEFAULT_COPYINFO_H_
#define DEFAULT_COPYINFO_H_

#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_icopycontrol.h>
#include "default_filecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class CopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(INode *node, const QByteArray &path);

	/* IFileContainer */
	virtual bool isDefault() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual const Location &location() const;
	virtual Location location(const IFileInfo *info) const;
	virtual Location location(const QString &fileName) const;

	virtual bool contains(const QString &fileName) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;

	virtual bool remove(const IFileInfo *info, QString &error) const;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const;
	virtual IFileContainer *create(const QString &fileName, QString &error) const;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;
	virtual const IApplications *applications() const;

	/* ICopyControl */
	virtual INode *node() const;
	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	FileContainer m_container;
	INode *m_node;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_COPYINFO_H_ */
