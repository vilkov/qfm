#ifndef FILESYSTEMIFILECONTAINERSCANNER_H_
#define FILESYSTEMIFILECONTAINERSCANNER_H_

#include "filesystemifileinfo.h"
#include "../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN
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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTAINERSCANNER_H_ */
