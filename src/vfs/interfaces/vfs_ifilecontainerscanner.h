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
#ifndef VFS_IFILECONTAINERSCANNER_H_
#define VFS_IFILECONTAINERSCANNER_H_

#include <tools/taskspool/task.h>
#include "vfs_ifileinfo.h"


VFS_NS_BEGIN
class Snapshot;
class SnapshotItem;
class IFileAccessor;


class IFileContainerScanner
{
public:
	typedef ::Tools::TasksPool::Task::Flags Flags;

public:
	class IEnumerator
	{
	public:
		typedef PScopedPointer<IEnumerator> Holder;

	public:
		virtual ~IEnumerator();

		virtual const IFileInfo *next() = 0;
		virtual IFileInfo *info() const = 0;
		virtual IFileAccessor *open(int mode, QString &error) const = 0;
	};

	class Filter
	{
	public:
		typedef PScopedPointer<Filter> Holder;

	public:
		virtual ~Filter();
		virtual bool match(const IFileInfo *info) const = 0;
	};

	struct Callback
	{
		typedef void (*Method)(void *user_data, SnapshotItem *item);

		Method method;
		void *user_data;
	};

	struct SearchArguments
	{
		Snapshot &snapshot;
		const Filter *filter;
		Callback callback;
		const volatile Flags &aborted;
	};

	struct ScanArguments
	{
		Snapshot &snapshot;
		const volatile Flags &aborted;
	};

public:
	virtual ~IFileContainerScanner();

	virtual IEnumerator *enumerate(QString &error) const = 0;
	virtual void scan(const ScanArguments &arguments, QString &error) const = 0;
	virtual void search(const SearchArguments &arguments, QString &error) const = 0;
};

VFS_NS_END

#endif /* VFS_IFILECONTAINERSCANNER_H_ */
