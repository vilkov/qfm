#ifndef VFS_IFILECONTAINERSCANNER_H_
#define VFS_IFILECONTAINERSCANNER_H_

#include "vfs_ifileinfo.h"
#include "../../tools/taskspool/task.h"


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
