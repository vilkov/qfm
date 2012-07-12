#ifndef VFS_IFILECONTAINERSCANNER_H_
#define VFS_IFILECONTAINERSCANNER_H_

#include "vfs_ifileinfo.h"
#include "../../tools/taskspool/task.h"


VFS_NS_BEGIN
class Snapshot;
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

public:
	virtual ~IFileContainerScanner();

	virtual IEnumerator *enumerate(QString &error) const = 0;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const = 0;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const = 0;
};

VFS_NS_END

#endif /* VFS_IFILECONTAINERSCANNER_H_ */
