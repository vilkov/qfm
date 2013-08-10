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
#ifndef DEFAULT_FILECONTAINERSCANNER_H_
#define DEFAULT_FILECONTAINERSCANNER_H_

#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN
class SnapshotItem;


class FileContainerScanner : public IFileContainerScanner
{
public:
	FileContainerScanner(const IFileContainer *container);

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

protected:
	void fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	void scan(SnapshotItem *root, const volatile Flags &aborted, QString &error) const;
	void search(const IFileContainer *container, const Callback &callback, const IFileContainerScanner::Filter *filter, const volatile Flags &aborted) const;

protected:
	const IFileContainer *m_container;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILECONTAINERSCANNER_H_ */
